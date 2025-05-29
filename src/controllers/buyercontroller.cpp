#include "buyercontroller.h"
#include <QFileInfo>
#include <QRegularExpression>

BuyerController::BuyerController(BuyerModel *model, QObject *parent)
    : QObject(parent)
    , model(model)
{
}

bool BuyerController::openFile(const QString &fileName)
{
    if (!model->isValidFile(fileName)) {
        emit error(tr("Неверный формат файла"));
        return false;
    }

    if (!model->loadFromFile(fileName)) {
        emit error(tr("Не удалось открыть файл"));
        return false;
    }

    currentFileName = fileName;
    emit fileOpened(fileName);
    return true;
}

bool BuyerController::saveFile(const QString &fileName)
{
    if (!model->saveToFile(fileName)) {
        emit error(tr("Не удалось сохранить файл"));
        return false;
    }

    currentFileName = fileName;
    emit fileSaved(fileName);
    return true;
}

bool BuyerController::saveFileAs(const QString &fileName)
{
    return saveFile(fileName);
}

QString BuyerController::getCurrentFileName() const
{
    return currentFileName;
}

bool BuyerController::addBuyer(const Buyer &buyer)
{
    if (!validateBuyer(buyer)) {
        emit error(tr("Неверные данные покупателя"));
        return false;
    }

    if (model->addBuyer(buyer)) {
        emit buyerAdded(model->rowCount() - 1);
        return true;
    }
    return false;
}

bool BuyerController::editBuyer(int row, const Buyer &buyer)
{
    if (!validateBuyer(buyer)) {
        emit error(tr("Неверные данные покупателя"));
        return false;
    }

    if (model->updateBuyer(row, buyer)) {
        emit buyerEdited(row);
        return true;
    }
    return false;
}

bool BuyerController::deleteBuyer(int row)
{
    if (model->removeBuyer(row)) {
        emit buyerDeleted(row);
        return true;
    }
    return false;
}

bool BuyerController::updateBuyer(int row, const Buyer &buyer)
{
    if (!validateBuyer(buyer)) {
        emit error(tr("Неверные данные покупателя"));
        return false;
    }

    if (model->updateBuyer(row, buyer)) {
        emit buyerEdited(row);
        return true;
    }
    return false;
}

Buyer BuyerController::getBuyer(int row) const
{
    return model->getBuyer(row);
}

QVector<Buyer> BuyerController::getAllBuyers() const
{
    return model->getAllBuyers();
}

QVector<Buyer> BuyerController::searchBuyers(const QString &searchText, int column) const
{
    QVector<Buyer> results = model->searchBuyers(searchText, column);
    return results;
}

void BuyerController::sortByColumn(int column, Qt::SortOrder order)
{
    model->sortByColumn(column, order);
    emit sortOrderChanged(column, order);
}

bool BuyerController::validateBuyer(const Buyer &buyer) const
{
    // Validate required fields
    if (buyer.lastName.isEmpty() || buyer.firstName.isEmpty() || buyer.middleName.isEmpty())
        return false;

    // Validate passport number format (e.g., "1234 567890")
    QRegularExpression passportRegex("^\\d{4}\\s\\d{6}$");
    if (!passportRegex.match(buyer.passportNumber).hasMatch())
        return false;

    // Validate phone number format (e.g., "+7 (999) 123-45-67")
    QRegularExpression phoneRegex("^\\+7\\s\\(\\d{3}\\)\\s\\d{3}-\\d{2}-\\d{2}$");
    if (!phoneRegex.match(buyer.phoneNumber).hasMatch())
        return false;

    // Validate email format
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!emailRegex.match(buyer.email).hasMatch())
        return false;

    // Validate property area (positive number)
    bool ok;
    double area = buyer.propertyArea.toDouble(&ok);
    if (!ok || area <= 0)
        return false;

    // Validate property price (positive number)
    double price = buyer.propertyPrice.toDouble(&ok);
    if (!ok || price <= 0)
        return false;

    // Validate purchase date format (YYYY-MM-DD)
    QRegularExpression dateRegex("^\\d{4}-\\d{2}-\\d{2}$");
    if (!dateRegex.match(buyer.purchaseDate).hasMatch())
        return false;

    // Contract number validation removed - any format is accepted

    return true;
}

bool BuyerController::isValidFile(const QString &fileName) const
{
    return model->isValidFile(fileName);
} 