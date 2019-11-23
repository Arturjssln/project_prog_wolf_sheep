 #include "Genome.hpp"
#include <Random/Random.hpp>
#include <iostream>
#include "Application.hpp"

//CONSTRUCTEUR
Genome::Genome(Genome* mere, Genome* pere) {
    /* Si l'utilisateur entre uniquement un parent on considerera
     * que c'est un animal sans parents */
    if(pere != nullptr and mere != nullptr) {
        randomSexe();
        randomColor(0, mere);
        randomColor(1, pere);
        for(size_t i(0); i < TAILLE_PROFIL_IMMU; ++i) {
            if(bernoulli(0.5)) {
                profilImmunitaire[i] = mere->profilImmunitaire[i];
            } else {
                profilImmunitaire[i] = pere->profilImmunitaire[i];
            }
            profilImmunitaire[i] += uniform(-getAppConfig().genome_mutability_innate_immmune_system,
                                              getAppConfig().genome_mutability_innate_immmune_system);
        }
    } else {
        randomSexe();
        randomColor(0);
        randomColor(1);
        for(size_t i(0); i < TAILLE_PROFIL_IMMU; ++i) {
            profilImmunitaire[i] = uniform(-5.0, 5.0);
        }
    }
}

/**************************************************************************************************************/
//METHODES GETTERS

Sex Genome::getSex() const {
	return genre;
}
ColorAllele Genome::getFirstColorAllele() const {
	return matGenetique[0];
}
ColorAllele Genome::getSecondColorAllele() const {
	return matGenetique[1];
}
ColorAllele Genome::getColorPhenotype() const {
	if( matGenetique[0] == BLACK and matGenetique[1] == BLACK)
		return BLACK;
	return WHITE;
}
double Genome::getImmuneGenes(size_t idx) const { //on suppose que l'indice donné est compris entre 0 et 9
	return profilImmunitaire[idx];
}

/**************************************************************************************************************/
//METHODES

//Méthode publique de la classe Genome qui l'affiche (utilisé pour la surcharge de l'opérateur <<)
void Genome::afficheGenome(std::ostream& sortie) const {
    switch(getSex()) {
        case MALE: sortie << "Genome (male) :" << std::endl;
            break;
        case FEMALE: sortie << "Genome (female) :" << std::endl;
            break;
    }
    sortie << "Color phenotype = ";
    afficheCouleur(sortie, getColorPhenotype());
    sortie << "First color allele = ";
    afficheCouleur(sortie, getFirstColorAllele());
    sortie << "Second color allele = ";
    afficheCouleur(sortie, getSecondColorAllele());
    sortie << "Immune profile = ";
    for(size_t i(0); i < TAILLE_PROFIL_IMMU; ++i) {
        sortie << getImmuneGenes(i) << " ";
    }
}

//Attribution aléatoire d'un genre
void Genome::randomSexe() {
    if(bernoulli(0.5)) {
        genre = MALE;
    } else {
        genre = FEMALE;
    }
}

/*Initialisation d'un allele (idx = 0 ou 1) du materiel genetique
 *(animal avec parents)*/
void Genome::randomColor(size_t idx, Genome* parent) {
    if(bernoulli(0.5)) {
        matGenetique[idx] = parent->getFirstColorAllele();
    } else {
        matGenetique[idx] = parent->getSecondColorAllele();
    }
}

/*Initialisation d'un allele (idx = 0 ou 1) du materiel genetique
 *(animal sans parents)*/
void Genome::randomColor(size_t idx) {
    if(bernoulli(getAppConfig().genome_black_allelle_prevalence)) {
        matGenetique[idx] = BLACK;
    } else {
        matGenetique[idx] = WHITE;
    }
}

//Fonction affichant la couleur sur le stream entré en paramètre
void afficheCouleur(std::ostream& sortie, ColorAllele couleur) {
	switch(couleur) {
		case BLACK: sortie << "black" << std::endl;
			break;
		case WHITE: sortie << "white" << std::endl;
			break;
        default: break;
		}
}

//SURCHARGE OPÉRATEUR
std::ostream& operator<<(std::ostream& sortie,const Genome& gen) {
    gen.afficheGenome(sortie);
	return sortie;
}
