#include "buyermodel.h"
#include <QDebug>

BuyerModel::BuyerModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    // Initialize headers
    headers << tr("Фамилия") << tr("Имя") << tr("Отчество")
            << tr("Номер паспорта") << tr("Номер телефона") << tr("Email")
            << tr("Тип недвижимости") << tr("Адрес недвижимости") << tr("Площадь недвижимости")
            << tr("Цена недвижимости") << tr("Дата покупки") << tr("Статус оплаты")
            << tr("Номер договора");
}

int BuyerModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return buyers.size();
}

int BuyerModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return COLUMN_COUNT;
}

QVariant BuyerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= buyers.size() || index.column() >= COLUMN_COUNT)
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        const Buyer &buyer = buyers[index.row()];
        switch (index.column()) {
            case 0: return buyer.lastName;
            case 1: return buyer.firstName;
            case 2: return buyer.middleName;
            case 3: return buyer.passportNumber;
            case 4: return buyer.phoneNumber;
            case 5: return buyer.email;
            case 6: return buyer.propertyType;
            case 7: return buyer.propertyAddress;
            case 8: return buyer.propertyArea;
            case 9: return buyer.propertyPrice;
            case 10: return buyer.purchaseDate;
            case 11: return buyer.paymentStatus;
            case 12: return buyer.contractNumber;
            default: return QVariant();
        }
    }
    return QVariant();
}

QVariant BuyerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal && section < headers.size())
        return headers[section];
    else if (orientation == Qt::Vertical)
        return section + 1;

    return QVariant();
}

bool BuyerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || !index.isValid() || index.row() >= buyers.size()) {
        return false;
    }

    Buyer &buyer = buyers[index.row()];
    bool success = true;

    switch (index.column()) {
        case 0: buyer.lastName = value.toString(); break;
        case 1: buyer.firstName = value.toString(); break;
        case 2: buyer.middleName = value.toString(); break;
        case 3: buyer.passportNumber = value.toString(); break;
        case 4: buyer.phoneNumber = value.toString(); break;
        case 5: buyer.email = value.toString(); break;
        case 6: buyer.propertyType = value.toString(); break;
        case 7: buyer.propertyAddress = value.toString(); break;
        case 8: buyer.propertyArea = value.toString(); break;
        case 9: buyer.propertyPrice = value.toString(); break;
        case 10: buyer.purchaseDate = value.toString(); break;
        case 11: buyer.paymentStatus = value.toString(); break;
        case 12: buyer.contractNumber = value.toString(); break;
        default: success = false;
    }

    if (success) {
        emit dataChanged(index, index);
    }

    return success;
}

Qt::ItemFlags BuyerModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool BuyerModel::loadFromFile(const QString &fileName)
{
    qDebug() << "Attempting to load file:" << fileName;
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << fileName << "Error:" << file.errorString();
        return false;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    beginResetModel();
    buyers.clear();

    // Read and validate header
    if (!in.atEnd()) {
        QString header = in.readLine().trimmed();
        if (header != "BUYER_DATABASE") {
            qDebug() << "Invalid file format. Expected BUYER_DATABASE header, got:" << header;
            file.close();
            return false;
        }
    }

    int lineNumber = 1;
    // Read data
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue; // Skip empty lines
        
        lineNumber++;
        
        QStringList fields = line.split("|");
        
        qDebug() << "Processing line" << lineNumber << "with" << fields.size() << "fields";
        qDebug() << "Raw line:" << line;
        
        if (fields.size() == COLUMN_COUNT) {
            Buyer buyer;
            buyer.lastName = fields[0].trimmed();
            buyer.firstName = fields[1].trimmed();
            buyer.middleName = fields[2].trimmed();
            buyer.passportNumber = fields[3].trimmed();
            buyer.phoneNumber = fields[4].trimmed();
            buyer.email = fields[5].trimmed();
            buyer.propertyType = fields[6].trimmed();
            buyer.propertyAddress = fields[7].trimmed();
            buyer.propertyArea = fields[8].trimmed();
            buyer.propertyPrice = fields[9].trimmed();
            buyer.purchaseDate = fields[10].trimmed();
            buyer.paymentStatus = fields[11].trimmed();
            buyer.contractNumber = fields[12].trimmed();
            buyers.append(buyer);
            
            // Debug output for each field
            qDebug() << "Loaded buyer:";
            qDebug() << "Name:" << buyer.lastName << buyer.firstName << buyer.middleName;
            qDebug() << "Contact:" << buyer.passportNumber << buyer.phoneNumber << buyer.email;
            qDebug() << "Property:" << buyer.propertyType << buyer.propertyAddress;
            qDebug() << "Area:" << buyer.propertyArea << "Price:" << buyer.propertyPrice;
            qDebug() << "Date:" << buyer.purchaseDate;
            qDebug() << "Status:" << buyer.paymentStatus << "Contract:" << buyer.contractNumber;
        } else {
            qDebug() << "Invalid line format at line" << lineNumber << ":" << line;
            qDebug() << "Expected" << COLUMN_COUNT << "fields, got" << fields.size();
            qDebug() << "Fields:" << fields;
        }
    }

    file.close();
    endResetModel();

    qDebug() << "Successfully loaded" << buyers.size() << "buyers from file:" << fileName;
    return buyers.size() > 0;
}

bool BuyerModel::saveToFile(const QString &fileName) const
{
    qDebug() << "Starting saveToFile operation for:" << fileName;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << fileName << "Error:" << file.errorString();
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // Write file format identifier
    out << "BUYER_DATABASE\n";
    qDebug() << "Wrote header to file";

    // Write data with | delimiter
    for (const Buyer &buyer : buyers) {
        // Create a list of fields
        QStringList fields;
        fields << buyer.lastName.trimmed()
               << buyer.firstName.trimmed()
               << buyer.middleName.trimmed()
               << buyer.passportNumber.trimmed()
               << buyer.phoneNumber.trimmed()
               << buyer.email.trimmed()
               << buyer.propertyType.trimmed()
               << buyer.propertyAddress.trimmed()
               << buyer.propertyArea.trimmed()  // Already formatted with 1 decimal place
               << buyer.propertyPrice.trimmed() // Already formatted as whole number
               << buyer.purchaseDate.trimmed()  // Already in YYYY-MM-DD format
               << buyer.paymentStatus.trimmed()
               << buyer.contractNumber.trimmed();

        // Join fields with | delimiter and write to file
        QString line = fields.join("|");
        qDebug() << "Saving line:" << line;
        out << line << "\n";
    }

    file.close();
    qDebug() << "Successfully saved" << buyers.size() << "buyers to file:" << fileName;
    return true;
}

bool BuyerModel::addBuyer(const Buyer &buyer)
{
    beginInsertRows(QModelIndex(), buyers.size(), buyers.size());
    buyers.append(buyer);
    endInsertRows();
    return true;
}

bool BuyerModel::removeBuyer(int row)
{
    if (row < 0 || row >= buyers.size()) {
        qDebug() << "Invalid row index for deletion:" << row;
        return false;
    }

    qDebug() << "Removing buyer at row:" << row;
    beginRemoveRows(QModelIndex(), row, row);
    buyers.removeAt(row);
    endRemoveRows();
    
    // Force update of all views
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
    return true;
}

bool BuyerModel::updateBuyer(int row, const Buyer &buyer)
{
    if (row < 0 || row >= buyers.size())
        return false;

    buyers[row] = buyer;
    emit dataChanged(index(row, 0), index(row, COLUMN_COUNT - 1));
    return true;
}

Buyer BuyerModel::getBuyer(int row) const
{
    if (row >= 0 && row < buyers.size())
        return buyers[row];
    return Buyer();
}

QVector<Buyer> BuyerModel::getAllBuyers() const
{
    return buyers;
}

bool BuyerModel::isValidFile(const QString &fileName) const
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    QString header = in.readLine();
    return header.startsWith("BUYER_DATABASE");
}

QVector<Buyer> BuyerModel::searchBuyers(const QString &searchText, int column) const
{
    QVector<Buyer> results;
    if (searchText.isEmpty()) {
        return results;
    }

    for (const Buyer &buyer : buyers) {
        // If column is specified, only search in that column
        if (column >= 0 && column < COLUMN_COUNT) {
            QString field;
            switch (column) {
                case 0: field = buyer.lastName; break;
                case 1: field = buyer.firstName; break;
                case 2: field = buyer.middleName; break;
                case 3: field = buyer.passportNumber; break;
                case 4: field = buyer.phoneNumber; break;
                case 5: field = buyer.email; break;
                case 6: field = buyer.propertyType; break;
                case 7: field = buyer.propertyAddress; break;
                case 8: field = buyer.propertyArea; break;
                case 9: field = buyer.propertyPrice; break;
                case 10: field = buyer.purchaseDate; break;
                case 11: field = buyer.paymentStatus; break;
                case 12: field = buyer.contractNumber; break;
            }
            if (field.contains(searchText, Qt::CaseInsensitive)) {
                results.append(buyer);
            }
        } else {
            // Global search across all fields
            if (buyer.lastName.contains(searchText, Qt::CaseInsensitive) ||
                buyer.firstName.contains(searchText, Qt::CaseInsensitive) ||
                buyer.middleName.contains(searchText, Qt::CaseInsensitive) ||
                buyer.passportNumber.contains(searchText, Qt::CaseInsensitive) ||
                buyer.phoneNumber.contains(searchText, Qt::CaseInsensitive) ||
                buyer.email.contains(searchText, Qt::CaseInsensitive) ||
                buyer.propertyType.contains(searchText, Qt::CaseInsensitive) ||
                buyer.propertyAddress.contains(searchText, Qt::CaseInsensitive) ||
                buyer.propertyArea.contains(searchText, Qt::CaseInsensitive) ||
                buyer.propertyPrice.contains(searchText, Qt::CaseInsensitive) ||
                buyer.purchaseDate.contains(searchText, Qt::CaseInsensitive) ||
                buyer.paymentStatus.contains(searchText, Qt::CaseInsensitive) ||
                buyer.contractNumber.contains(searchText, Qt::CaseInsensitive)) {
                results.append(buyer);
            }
        }
    }
    return results;
}

void BuyerModel::sortByColumn(int column, Qt::SortOrder order)
{
    if (column < 0 || column >= COLUMN_COUNT)
        return;

    beginResetModel();
    std::sort(buyers.begin(), buyers.end(), [column, order](const Buyer &a, const Buyer &b) {
        QString fieldA, fieldB;
        switch (column) {
            case 0: fieldA = a.lastName; fieldB = b.lastName; break;
            case 1: fieldA = a.firstName; fieldB = b.firstName; break;
            case 2: fieldA = a.middleName; fieldB = b.middleName; break;
            case 3: fieldA = a.passportNumber; fieldB = b.passportNumber; break;
            case 4: fieldA = a.phoneNumber; fieldB = b.phoneNumber; break;
            case 5: fieldA = a.email; fieldB = b.email; break;
            case 6: fieldA = a.propertyType; fieldB = b.propertyType; break;
            case 7: fieldA = a.propertyAddress; fieldB = b.propertyAddress; break;
            case 8: fieldA = a.propertyArea; fieldB = b.propertyArea; break;
            case 9: fieldA = a.propertyPrice; fieldB = b.propertyPrice; break;
            case 10: fieldA = a.purchaseDate; fieldB = b.purchaseDate; break;
            case 11: fieldA = a.paymentStatus; fieldB = b.paymentStatus; break;
            case 12: fieldA = a.contractNumber; fieldB = b.contractNumber; break;
        }
        return order == Qt::AscendingOrder ? fieldA < fieldB : fieldA > fieldB;
    });
    endResetModel();
}

void BuyerModel::clear() {
    beginResetModel();
    buyers.clear();
    endResetModel();
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

bool BuyerModel::rowMatchesSearch(int row, const QString &searchText) const
{
    if (row < 0 || row >= buyers.size() || searchText.isEmpty()) {
        return false;
    }

    const Buyer &buyer = buyers[row];
    return buyer.lastName.contains(searchText, Qt::CaseInsensitive) ||
           buyer.firstName.contains(searchText, Qt::CaseInsensitive) ||
           buyer.middleName.contains(searchText, Qt::CaseInsensitive) ||
           buyer.passportNumber.contains(searchText, Qt::CaseInsensitive) ||
           buyer.phoneNumber.contains(searchText, Qt::CaseInsensitive) ||
           buyer.email.contains(searchText, Qt::CaseInsensitive) ||
           buyer.propertyType.contains(searchText, Qt::CaseInsensitive) ||
           buyer.propertyAddress.contains(searchText, Qt::CaseInsensitive) ||
           buyer.propertyArea.contains(searchText, Qt::CaseInsensitive) ||
           buyer.propertyPrice.contains(searchText, Qt::CaseInsensitive) ||
           buyer.purchaseDate.contains(searchText, Qt::CaseInsensitive) ||
           buyer.paymentStatus.contains(searchText, Qt::CaseInsensitive) ||
           buyer.contractNumber.contains(searchText, Qt::CaseInsensitive);
} 