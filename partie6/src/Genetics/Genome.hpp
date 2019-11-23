#include <array>
#include <sstream>

#ifndef GENOME_H
#define GENOME_H

//Declaration constante globale : facilité maintenance
const size_t TAILLE_PROFIL_IMMU(10);

enum Sex{MALE, FEMALE};
enum ColorAllele{WHITE, BLACK};

class Genome {
    public:
        //CONSTRUCTEUR
        Genome(Genome* pere = nullptr, Genome* mere = nullptr);

        //DESTRUCTEUR
        ~Genome() = default;

        //MÉTHODES
        Sex getSex() const;
        ColorAllele getFirstColorAllele() const;
        ColorAllele getSecondColorAllele() const;
        ColorAllele getColorPhenotype() const;
        double getImmuneGenes(size_t idx) const;
        void afficheGenome(std::ostream& sortie) const;
        void afficheImmuneProfile(std::ostream& out) const;
        void setProfilImmunitaire(std::array<double, TAILLE_PROFIL_IMMU> const& profil);

    private:
        //ATTRIBUTS
        Sex gender;
        std::array<ColorAllele,2> matGenetique;
        std::array<double, TAILLE_PROFIL_IMMU> profilImmunitaire;

        //MÉTHODES
        void randomSexe();
        void randomColor(size_t idx, Genome* parent);
        void randomColor(size_t idx);
};

//FONCTION
void afficheCouleur(std::ostream& sortie, ColorAllele couleur);

//SURCHARGE OPÉRATEUR
std::ostream& operator<<(std::ostream& sortie,const Genome& gen);

#endif
