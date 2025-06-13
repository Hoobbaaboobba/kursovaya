#ifndef SEARCHHIGHLIGHTPROXY_H
#define SEARCHHIGHLIGHTPROXY_H

#include <QSortFilterProxyModel>
#include <QColor>

class SearchHighlightProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit SearchHighlightProxy(QObject *parent = nullptr);

    void setSearchText(const QString &text);
    QString searchText() const { return m_searchText; }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QString m_searchText;
    static const QColor HIGHLIGHT_COLOR;
};

#endif // SEARCHHIGHLIGHTPROXY_H 