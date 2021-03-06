#include "MotCleDelegate.h"

QWidget * MotCleDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if(TreeModelMotCle::NbrColumnMotCle <= index.column() && index.column() < TreeModelMotCle::NbrColumnMotCle + TreeModelMotCle::NbrIndex)
    {
        QComboBox * editor = new QComboBox(parent);
        editor->addItem("Interdit",bdd::motClePermissionNum::InterditMCNum);
        editor->addItem("Permis",bdd::motClePermissionNum::PermisMCNum);
        editor->setAutoFillBackground(true);
        return editor;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

bool MotCleDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent * mouseEvent = static_cast<QMouseEvent *>(event);
        if(mouseEvent->button() == Qt::RightButton)
        {
            showContextMenu(model,index,mouseEvent->globalPos());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}

/*void MotCleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(TreeModelMotCle::NbrColumnMotCle <= index.column() && index.column() < TreeModelMotCle::NbrColumnMotCle + TreeModelMotCle::NbrIndex)
    {
        painter->save();
        switch (index.data().toInt()) {
        case bdd::motClePermissionNum::InterditMCNum:
            painter->fillRect(option.rect, QColor(Qt::red));
            break;
        case bdd::motClePermissionNum::PermisMCNum:
            painter->fillRect(option.rect, QColor(Qt::green));
        default:
            break;
        }
        painter->restore();
    }
    else
        QStyledItemDelegate::paint(painter,option,index);
}*/

void MotCleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(TreeModelMotCle::NbrColumnMotCle <= index.column() && index.column() < TreeModelMotCle::NbrColumnMotCle + TreeModelMotCle::NbrIndex)
    {
        QComboBox * comboBox = static_cast<QComboBox *>(editor);
        comboBox->setCurrentIndex(comboBox->findData(index.data(Qt::EditRole)));
    }
    else
        QStyledItemDelegate::setEditorData(editor,index);
}

void MotCleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(TreeModelMotCle::NbrColumnMotCle <= index.column() && index.column() < TreeModelMotCle::NbrColumnMotCle + TreeModelMotCle::NbrIndex)
        model->setData(index,static_cast<QComboBox *>(editor)->currentData());
    else
        QStyledItemDelegate::setModelData(editor,model,index);
}

void MotCleDelegate::showContextMenu(QAbstractItemModel *model, const QModelIndex &index, const QPoint& globalPos)
{
    QMenu menu;
    QAction * addSon = menu.addAction(tr("Ajouter un fils"));
    QAction * addBro = menu.addAction(tr("Ajouter un frère"));

    connect(addSon,&QAction::triggered,[model,&index]{static_cast<TreeModelMotCle *>(model)->insertRow(0,index);});
    connect(addBro,&QAction::triggered,[model,&index]{static_cast<TreeModelMotCle *>(model)->insertRow(index.row()+1,index.parent());});
    if(static_cast<TreeModelMotCle *>(model)->autorisation(index,bdd::Autorisation::Suppr))
    {
        menu.addSeparator();
        QAction * delMotCle = menu.addAction(QString(tr("Supprimer le mot clé ")).append(model->data(model->index(index.row(),TreeModelMotCle::NomMotCleIndex,index.parent())).toString()));
        connect(delMotCle,&QAction::triggered,[model,&index]{static_cast<TreeModelMotCle *>(model)->removeRow(index.row(),index.parent());});
    }

    menu.exec(globalPos);
}
