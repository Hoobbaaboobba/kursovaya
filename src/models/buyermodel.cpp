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
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    if (index.row() >= buyers.size() || index.column() >= COLUMN_COUNT)
        return false;

    Buyer &buyer = buyers[index.row()];
    QString strValue = value.toString();

    switch (index.column()) {
        case 0: buyer.lastName = strValue; break;
        case 1: buyer.firstName = strValue; break;
        case 2: buyer.middleName = strValue; break;
        case 3: buyer.passportNumber = strValue; break;
        case 4: buyer.phoneNumber = strValue; break;
        case 5: buyer.email = strValue; break;
        case 6: buyer.propertyType = strValue; break;
        case 7: buyer.propertyAddress = strValue; break;
        case 8: buyer.propertyArea = strValue; break;
        case 9: buyer.propertyPrice = strValue; break;
        case 10: buyer.purchaseDate = strValue; break;
        case 11: buyer.paymentStatus = strValue; break;
        case 12: buyer.contractNumber = strValue; break;
        default: return false;
    }

    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags BuyerModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool BuyerModel::loadFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    // Check file format
    QString header = in.readLine();
    if (!header.startsWith("BUYER_DATABASE"))
        return false;

    beginResetModel();
    buyers.clear();

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty())
            continue;

        QStringList fields = line.split("|");
        if (fields.size() != COLUMN_COUNT)
            continue;

        Buyer buyer;
        buyer.lastName = fields[0];
        buyer.firstName = fields[1];
        buyer.middleName = fields[2];
        buyer.passportNumber = fields[3];
        buyer.phoneNumber = fields[4];
        buyer.email = fields[5];
        buyer.propertyType = fields[6];
        buyer.propertyAddress = fields[7];
        buyer.propertyArea = fields[8];
        buyer.propertyPrice = fields[9];
        buyer.purchaseDate = fields[10];
        buyer.paymentStatus = fields[11];
        buyer.contractNumber = fields[12];

        buyers.append(buyer);
    }

    endResetModel();
    return true;
}

bool BuyerModel::saveToFile(const QString &fileName) const
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // Write file format identifier
    out << "BUYER_DATABASE\n";

    for (const Buyer &buyer : buyers) {
        out << buyer.lastName << "|"
            << buyer.firstName << "|"
            << buyer.middleName << "|"
            << buyer.passportNumber << "|"
            << buyer.phoneNumber << "|"
            << buyer.email << "|"
            << buyer.propertyType << "|"
            << buyer.propertyAddress << "|"
            << buyer.propertyArea << "|"
            << buyer.propertyPrice << "|"
            << buyer.purchaseDate << "|"
            << buyer.paymentStatus << "|"
            << buyer.contractNumber << "\n";
    }

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
    if (row < 0 || row >= buyers.size())
        return false;

    beginRemoveRows(QModelIndex(), row, row);
    buyers.removeAt(row);
    endRemoveRows();
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
    if (column < 0 || column >= COLUMN_COUNT)
        return results;

    for (const Buyer &buyer : buyers) {
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
        if (field.contains(searchText, Qt::CaseInsensitive))
            results.append(buyer);
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