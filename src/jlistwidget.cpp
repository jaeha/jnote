#include "jlistwidget.h"


#define LIST_KEY    1001

JListWidget::JListWidget(QWidget *parent) : QListWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setContextMenuPolicy(Qt::CustomContextMenu);
   /*
       QVBoxLayout *vbox = new QVBoxLayout();
       vbox->addWidget(m_list);
       vbox->setMargin(2);
       vbox->setSpacing(2);
       setLayout(vbox);
   */

   connect(this, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), \
           this, SLOT(onCurrentItemChanged(QListWidgetItem*, QListWidgetItem*)));
   connect(parent, SIGNAL(toFontResize(int)), this, SLOT(onFontResize(int)));
}


void JListWidget::onCurrentItemChanged(QListWidgetItem* newItem, QListWidgetItem* oldItem)
{
   // message(DEBUG, "JListWidget::onCurrentItemChanged()", QString("items: %1").arg(selectedItems().size()));

    if (newItem == NULL) {
        emit toItemChanged(NO_DATA);
    } else {
        message(DEBUG, "onCurrentItemChanged()", "item changed");
        emit toItemChanged(newItem->data(LIST_KEY).toInt());
    }
}

QList<int> JListWidget::selectedData()
{
    QList<int> selected;
    foreach (QModelIndex index, selectedIndexes()){
        selected.append(index.data(LIST_KEY).toInt());
    }

    return selected;
}

void JListWidget::updateAllData(IdMap map)
{
    clear();

    if (map.count() == 0)
        return;

    IdMapIterator i(map);
    while (i.hasNext()) {
        i.next();
        QListWidgetItem *item = new QListWidgetItem(i.value());
        item->setData(LIST_KEY, i.key());
        addItem(item);
    }

    setCurrentRow(0);
}

void JListWidget::addData(int id, QString str)
{
    selectionModel()->clearSelection();

    // add new item
    QListWidgetItem *item = new QListWidgetItem(str);
    item->setData(LIST_KEY, id);
    addItem(item);
    setCurrentItem(item);
}

void JListWidget::removeSelectedData()
{
    message(DEBUG, "removeSelectedData()", QString("remove: %1").arg(currentRow()));
    takeItem(currentRow());
}

void JListWidget::updateCurrentData(QString str)
{
    currentItem()->setText(str);
}

void JListWidget::onFontResize(int fontsize)
{
 //   message(DEBUG, "onFontResize()", QString("new font size: %1").arg(fontsize));
    setStyleSheet(FONT_STYLE_LIST(fontsize));
}

int JListWidget::currentID()
{
    if (currentItem())
        return currentItem()->data(LIST_KEY).toInt();
    else
        return NO_DATA;
}
