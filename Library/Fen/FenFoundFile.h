/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/02/2016
 */

#ifndef FENFOUNDFILE_H
#define FENFOUNDFILE_H

#include <QApplication>
#include <QFileDialog>
#include <QMap>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <stdexcept>
#include "Fen.h"
#include "../Div/Config.h"
#include "../Div/FileInterface.h"

/*! \ingroup groupeFen
 * \brief Cette classe est chargée de la vérifiaction des chemins des fichiers et de leur création s'il sont introuvables.
 */

class FenFoundFile : public Fen
{
protected:
    FileInterface * m_ptr_file; //!< Pointeur sur le gestionnaire du fichier que l'on souhaite ouvrrir.

public:
    //! Constructeur. Donner en argument un pointeur sur le gestonnaire de fichier que l'on souhaite ouvrir.
    FenFoundFile(FileInterface *file, QWidget *parent = 0);

    //! Test si le fichier existe, si oui le chemin du fichier est renvoyé sinon une fenêtre de dialogue est ouverte.
    //! Par défaut une copie du fichier est effectuée, mettre copy=false pour ne pas copier le fichier.
    QString exists(bool copy = true);

    //QString fileInvalid();
    //QString fileNoteExist();
    //const QString & filePath();

    //! Test si le fichier est valide, si oui le chemin du fichier est renvoyé sinon une fenêtre de dialogue est ouverte.
    //! Par défaut une copie du fichier est effectuée, mettre copy=false pour ne pas copier le fichier.
    QString isValid(bool copy = true);

    /*!
     * \brief Ouvre un fichier à partir d'un chemin XML dans le fichier de configuration.
     */
    void openInConf(Config *config, const QString & path, bool save = true);

protected:
    /*!
     * \brief Affiche une première fenêtre de choix binaire puis une seconde d'ouverture de fichier.
     * Cette séquence est répétée jusqu'à obtenir un choix valide.
     *
     * Affiche une première fenêtre de choix binaire:
     *  + si le bouton de texte accept est cliqué, une fenêtre d'ouverture de fichier est ouverte.
     *  + si le bouton de texte reject est cliqué, une QString vide est renvoyée.
     *
     * Cette séquence est répétée de manière récursive jusqu'à obtenir un choix valide.
     * \param text Texte de la première fenêtre
     * \param fileName Chemin du fichier
     * \param fileExtension Extension du fichier
     * \param accept Texte du boutton accepte
     * \param reject Texte du boutton rejecte
     * \param openText Texte de la seconde fenêtre d'ouverture de fichier
     * \return Chemin du nouveau fichier
     */
    QString foundFileMessage(const QString & text, const QString &fileName, const QString &fileExtension, const QString & accept = "Créer", const QString & reject = "Trouver", const QString & openText = "Indiquez le nom d'un fichier");

    /*//! Formate une QString associant le chemin du fichier path et les attributs atts pour un message d'erreur.
    QString inputToQString(const QString & path, const QMap<QString,QString> & atts);
    */

    //! Affiche une QMessageBox modale de texte text avec des bouttons de texte accept et de texte reject.
    //! Retourne true si le bouton de texte accept est cliqué et false sinon.
    bool message(const QString & text, const QString & accept, const QString & reject);
};

#endif // FENFOUNDFILE_H
