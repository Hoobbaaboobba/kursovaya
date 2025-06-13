#include "searchhighlightproxy.h"
#include "buyermodel.h"

const QColor SearchHighlightProxy::HIGHLIGHT_COLOR = QColor(255, 255, 0, 127); // Semi-transparent yellow

SearchHighlightProxy::SearchHighlightProxy(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

void SearchHighlightProxy::setSearchText(const QString &text)
{
    if (m_searchText != text) {
        m_searchText = text;
        emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
    }
}

QVariant SearchHighlightProxy::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::BackgroundRole && !m_searchText.isEmpty()) {
        QModelIndex sourceIndex = mapToSource(index);
        BuyerModel* model = qobject_cast<BuyerModel*>(sourceModel());
        if (model && model->rowMatchesSearch(sourceIndex.row(), m_searchText)) {
            return HIGHLIGHT_COLOR;
        }
    }

    return QSortFilterProxyModel::data(index, role);
} 