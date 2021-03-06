#include "NewEtablissementDialog.h"

NewEtablissementDialog::NewEtablissementDialog(const VectorPtr<TypeEtablissement> & vectTpEtab, const VectorPtr<Niveau> & vectNiveau,  const QString &nc, const QString &nom, QWidget *parent)
    : QDialog(parent),
      m_vectTpEtab(vectTpEtab),
      m_vectNiveau(vectNiveau)
{
    // Nom
    m_labelNc = new QLabel(tr("Nom Abrégé de l'établissement :"));
    m_labelNom = new QLabel(tr("Nom de l'établissement :"));
    m_lineNc = new QLineEdit(nc);
    m_lineNom = new QLineEdit(nom);

    m_layoutNom = new QVBoxLayout();
    m_layoutNom->addWidget(m_labelNom);
    m_layoutNom->addWidget(m_lineNom);
    m_layoutNom->addWidget(m_labelNc);
    m_layoutNom->addWidget(m_lineNc);

    // Adresse
    m_labelCP = new QLabel(tr("Code Postal :"));
    m_labelNuRue = new QLabel(tr("n° de rue :"));
    m_labelPays = new QLabel(tr("Pays :"));
    m_labelRue = new QLabel(tr("Rue :"));
    m_labelVille =new QLabel(tr("Ville :"));
    m_lineCp = new QLineEdit();
    m_lineNuRue = new QLineEdit();
    m_linePays = new QLineEdit();
    m_linePays->setText("France");
    m_lineRue = new QLineEdit();
    m_lineVille = new QLineEdit();

    m_layoutAdr = new QGridLayout();
    m_layoutAdr->addWidget(m_labelNuRue,0,0);
    m_layoutAdr->addWidget(m_lineNuRue,0,1);
    m_layoutAdr->addWidget(m_labelRue,0,2);
    m_layoutAdr->addWidget(m_lineRue,0,3);
    m_layoutAdr->addWidget(m_labelCP,1,0);
    m_layoutAdr->addWidget(m_lineCp,1,1);
    m_layoutAdr->addWidget(m_labelVille,1,2);
    m_layoutAdr->addWidget(m_lineVille,1,3);
    m_layoutAdr->addWidget(m_labelPays,1,4);
    m_layoutAdr->addWidget(m_linePays,1,5);

    m_groupAdr = new QGroupBox("Adresse de l'établissement :");
    m_groupAdr->setLayout(m_layoutAdr);

    // Bouton
    m_buttonCancel = new QPushButton(tr("Annuler"));
    connect(m_buttonCancel,&QPushButton::clicked,this,&QDialog::reject);

    m_buttonCreer = new QPushButton(tr("Créer"));
    connect(m_buttonCreer,&QPushButton::clicked,this,&QDialog::accept);

    m_layoutBouton = new QVBoxLayout();
    m_layoutBouton->addWidget(m_buttonCreer,Qt::AlignTop);
    m_layoutBouton->addWidget(m_buttonCancel);
    m_layoutBouton->addStretch();

    //Haut
    m_layoutHaut = new QHBoxLayout();
    m_layoutHaut->addLayout(m_layoutNom);
    m_layoutHaut->addLayout(m_layoutBouton);

    //Choix Type
    m_layoutChoixType = new QGridLayout();
    m_signalChoixType = new QSignalMapper(this);
    int j = 0;
    for(VectorPtr<TypeEtablissement>::iterator i = m_vectTpEtab.begin(); i != m_vectTpEtab.end(); ++i, ++j)
    {
       QCheckBox *box = new QCheckBox((*i).nom());
       connect(box, SIGNAL(stateChanged(int)), m_signalChoixType, SLOT(map()));
       m_signalChoixType->setMapping(box,(*i).id());
       m_layoutChoixType->addWidget(box, j / NbrChoixLine, j % NbrChoixLine);
    }
    connect(m_signalChoixType, SIGNAL(mapped(int)),this, SLOT(boxCheckChange(int)));

    m_groupChoixType = new QGroupBox(tr("Le type de classes présentes dans l'établissement:"));
    m_groupChoixType->setLayout(m_layoutChoixType);


    //Liste de classe
    m_selectListeClasse = new SelectInListBox(tr("Séledtion de classes:"),tr("Classes présentes dans l'établissement:"), false, false);

    //Main Layout
    m_layoutMain = new QVBoxLayout(this);
    m_layoutMain->addLayout(m_layoutHaut);
    m_layoutMain->addWidget(m_groupAdr);
    m_layoutMain->addWidget(m_groupChoixType);
    m_layoutMain->addWidget(m_selectListeClasse);
}

void NewEtablissementDialog::boxCheckChange(int n)
{
    if(static_cast<QCheckBox *>(m_signalChoixType->mapping(n))->isChecked())
    {
        for(VectorPtr<Niveau>::iterator i = m_vectNiveau.begin(); i != m_vectNiveau.end(); ++i)
        {
            if((*i).idTpEtab() == n)
                m_selectListeClasse->append((*i).id(),(*i).nom(),false);
        }
    }
    else
    {
        for(VectorPtr<Niveau>::iterator i = m_vectNiveau.begin(); i != m_vectNiveau.end(); ++i)
        {
            if((*i).idTpEtab() == n)
                m_selectListeClasse->remove((*i).id(),(*i).nom());
        }
    }
}

NewEtablissementDialog::dialogResult NewEtablissementDialog::value() const
{
     NewEtablissementDialog::dialogResult result;
     result.etab = Etablissement(m_lineNc->text(),m_lineNom->text());
     result.niveaux = m_selectListeClasse->value().second;
     for(VectorPtr<TypeEtablissement>::iterator i = m_vectTpEtab.begin(); i != m_vectTpEtab.end(); ++i)
     {
         if(static_cast<QCheckBox *>(m_signalChoixType->mapping((*i).id()))->isChecked())
            result.types.append((*i).id());
     }
     result.donnee.resize(adresse::Nbr);
     result.donnee[adresse::CodePostalInd] = QVariant(m_lineCp->text());
     result.donnee[adresse::NumRueInd] = QVariant(m_lineNuRue->text());
     result.donnee[adresse::PaysInd] = QVariant(m_linePays->text());
     result.donnee[adresse::RueInd] = QVariant(m_lineRue->text());
     result.donnee[adresse::VilleInd] = QVariant(m_lineVille->text());
     return result;
}
