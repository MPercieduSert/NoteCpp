#include "SelectDonneeDialog.h"

SelectDonneeDialog::SelectDonneeDialog(const Tree<Donnee> &tree, QWidget *parent)
    : QDialog(parent)
{
    m_label = new QLabel(tr("Séléctionner une donnée:"));
    m_cancelBouton = new QPushButton(tr("Annuler"));
    m_okBouton = new QPushButton(tr("OK"));
    m_treeWidget = new ReadTreeWidget<Donnee>(tree);

    connect(m_cancelBouton,&QPushButton::clicked,this,&SelectDonneeDialog::reject);
    connect(m_okBouton,&QPushButton::clicked,this,&SelectDonneeDialog::accept);

    m_hLayout = new QHBoxLayout();
    m_hLayout->addWidget(m_okBouton);
    m_hLayout->addWidget(m_cancelBouton);

    m_vLayout = new QVBoxLayout(this);
    m_vLayout->addWidget(m_label);
    m_vLayout->addWidget(m_treeWidget);
    m_vLayout->addLayout(m_hLayout);
}

void SelectDonneeDialog::accept()
{
    QDialog::accept();
    setResult(m_treeWidget->value());
}

void SelectDonneeDialog::reject()
{
    QDialog::reject();
    setResult(0);
}