#ifndef BUYERCONTROLLER_H
#define BUYERCONTROLLER_H

#include <QObject>
#include <QString>
#include <QVector>
#include "../models/buyermodel.h"

class BuyerController : public QObject
{
    Q_OBJECT

public:
    explicit BuyerController(BuyerModel *model, QObject *parent = nullptr);

    // File operations
    bool openFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    bool saveFileAs(const QString &fileName);
    QString getCurrentFileName() const;

    // Buyer operations
    bool addBuyer(const Buyer &buyer);
    bool editBuyer(int row, const Buyer &buyer);
    bool updateBuyer(int row, const Buyer &buyer);
    bool deleteBuyer(int row);
    Buyer getBuyer(int row) const;
    QVector<Buyer> getAllBuyers() const;

    // Search and sort operations
    QVector<Buyer> searchBuyers(const QString &searchText, int column) const;
    void sortByColumn(int column, Qt::SortOrder order);

    // Validation
    bool validateBuyer(const Buyer &buyer) const;
    bool isValidFile(const QString &fileName) const;

signals:
    void fileOpened(const QString &fileName);
    void fileSaved(const QString &fileName);
    void error(const QString &message);
    void buyerAdded(int row);
    void buyerEdited(int row);
    void buyerDeleted(int row);
    void searchResultsUpdated(const QVector<Buyer> &results);
    void sortOrderChanged(int column, Qt::SortOrder order);
    void dataChanged();

private:
    BuyerModel *model;
    QString currentFileName;
};

#endif // BUYERCONTROLLER_H 