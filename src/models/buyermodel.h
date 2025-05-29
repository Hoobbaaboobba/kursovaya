#ifndef BUYERMODEL_H
#define BUYERMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct Buyer {
    QString lastName;
    QString firstName;
    QString middleName;
    QString passportNumber;
    QString phoneNumber;
    QString email;
    QString propertyType;
    QString propertyAddress;
    QString propertyArea;
    QString propertyPrice;
    QString purchaseDate;
    QString paymentStatus;
    QString contractNumber;
};

class BuyerModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit BuyerModel(QObject *parent = nullptr);

    // QAbstractTableModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Custom methods
    bool loadFromFile(const QString &fileName);
    bool saveToFile(const QString &fileName) const;
    bool addBuyer(const Buyer &buyer);
    bool removeBuyer(int row);
    bool updateBuyer(int row, const Buyer &buyer);
    Buyer getBuyer(int row) const;
    QVector<Buyer> getAllBuyers() const;
    bool isValidFile(const QString &fileName) const;
    QVector<Buyer> searchBuyers(const QString &searchText, int column) const;
    void sortByColumn(int column, Qt::SortOrder order);

private:
    QVector<Buyer> buyers;
    QStringList headers;
    static const int COLUMN_COUNT = 13;
};

#endif // BUYERMODEL_H 