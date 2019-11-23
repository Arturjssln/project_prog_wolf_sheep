/*
 * prjsv 2016
 * 2016
 * Marco Antognini
 */

#include <Application.hpp>
#include <Genetics/Genome.hpp>
#include <Random/Random.hpp>
#include <Env/Obstacle.hpp>

#include <catch.hpp>

#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>


SCENARIO("Creating Genomes", "[Genome]")
{
    GIVEN("1000 newborns, no parents")
    {
		constexpr int SAMPLE_SIZE(1000);
        std::vector<Genome> newBorns;
		int femaleCount(0);
		int alleleCount1(0);
		int alleleCount2(0);
		bool wrongPhenotype(false);
		bool wrongImmuneProfileValue(false);
		int immuneProfileHasNegative(0);
		int immuneProfileHasPositive(0);
		int immuneProfileIsNotInt(0);
        // create 1000 Genome from scratch
        for (size_t i(0); i < SAMPLE_SIZE; ++i){
            newBorns.emplace_back(Genome());
			Genome o(newBorns.back());
			femaleCount += (o.getSex() == Sex::FEMALE) ? 1 : 0;
			 alleleCount1 += (o.getFirstColorAllele() == ColorAllele::BLACK) ? 1 : 0;
			 alleleCount2 += (o.getSecondColorAllele() == ColorAllele::BLACK) ? 1 : 0;
			 if ((o.getFirstColorAllele() == ColorAllele::WHITE || o.getSecondColorAllele() == ColorAllele::WHITE) && o.getColorPhenotype() != ColorAllele::WHITE){
				 wrongPhenotype = true;
			 }
			 // only test the first 10%
			 if (i < SAMPLE_SIZE/10){
				 for (size_t j(0); j < 10; ++j){
					 if ((o.getImmuneGenes(j) < -5) || (o.getImmuneGenes(j) > 5)){
						 wrongImmuneProfileValue = true;
					 }

					 if (o.getImmuneGenes(j) >= 0){
						 ++immuneProfileHasPositive;
					 }
			 
					 if (o.getImmuneGenes(j) < 0){
						 ++immuneProfileHasNegative;
					 }
					 if (o.getImmuneGenes(j) != std::floor(o.getImmuneGenes(j))){
						 ++immuneProfileIsNotInt;
					 }
				 }
			 }
		}
	

		THEN("They can be printed"){
			// simple check of overloaded operator:
			std::cout << Genome() << std::endl;
		}
		

        THEN("About 50% females are born")
        {
              //false negatives should seldom happen
			// May be lower a bit the expectations >=300 <=700 ??
             CHECK(femaleCount >= 400);
             CHECK(femaleCount <= 600);
        }

         THEN("About 30% black alleles for each chromosome")
         {
             CHECK(alleleCount1 >= 200);
             CHECK(alleleCount1 <= 400);
             CHECK(alleleCount2 >= 200);
             CHECK(alleleCount2 <= 400);
         }
         
         THEN("Black is a recessive phenotype")
         {
 			 CHECK_FALSE(wrongPhenotype);
         }
         
         THEN("Immune profile of 10 random doubles between -5 and 5")
         {
 			 CHECK_FALSE(wrongImmuneProfileValue);
			 CHECK(immuneProfileHasNegative > 0);
			 CHECK(immuneProfileHasPositive > 0);
			 CHECK(immuneProfileIsNotInt > 0);    
 		 }
		 
    }

    GIVEN("Newborns, two parents")
    {
		constexpr int SAMPLE_SIZE(300);
        std::vector<int> inheritedFromParents1(10,0);
        bool immuneNotFromParent(false);
        bool immuneInheritanceDesiquilibrium(false);
        int inheritedMotherAllele1(0);
        int inheritedFatherAllele1(0);
        bool colorAllele1NotFromCorrectParent(false); 
        bool colorAllele2NotFromCorrectParent(false); 
        
        // create 600 parents from scratch and 300 children
        std::vector<Genome> parents1;
        std::vector<Genome> parents2;
        std::vector<Genome> children;
        for (size_t i(0); i < SAMPLE_SIZE; ++i){
            parents1.emplace_back(Genome());
            parents2.emplace_back(Genome());
            children.emplace_back(Genome(&parents1[i], &parents2[i]));
        }
		
        for (size_t c(0); c < SAMPLE_SIZE; ++c)
            {
                for (size_t i(0); i < 10; ++i)
                {
                    if (std::abs(children[c].getImmuneGenes(i) - parents1[c].getImmuneGenes(i)) <= 0.3) // take into account variability due to random mutations
                        ++inheritedFromParents1[i];
                    else if (std::abs(children[c].getImmuneGenes(i) - parents2[c].getImmuneGenes(i) > 0.3))
                        immuneNotFromParent = true;
                }
            }
            
        for (size_t i(0); i < 10; ++i)
        {
             if (inheritedFromParents1[i] < 30 || inheritedFromParents1[i] > 270) // is there a blatant disequilibrium ?
                 immuneInheritanceDesiquilibrium = true;
        }
             
        for (size_t c(0); c < SAMPLE_SIZE; ++c)
        {
            
            if (children[c].getFirstColorAllele() == parents1[c].getFirstColorAllele())
                inheritedMotherAllele1++;
            else if (children[c].getFirstColorAllele() != parents1[c].getSecondColorAllele())
                colorAllele1NotFromCorrectParent = true;
            
            if (children[c].getSecondColorAllele() == parents2[c].getFirstColorAllele())
                inheritedFatherAllele1++;
            else if (children[c].getSecondColorAllele() != parents2[c].getSecondColorAllele())
                colorAllele2NotFromCorrectParent = true;
        }
       
        THEN("Each component of immuneGenes is equiprobably inherited from one parent"){
            CHECK_FALSE(immuneNotFromParent);
            CHECK_FALSE(immuneInheritanceDesiquilibrium);
        }

        THEN("The newborn inherits the first ColorAllele from his mother (parent1) and the second from his father (parent2)")
        {
            CHECK_FALSE(colorAllele1NotFromCorrectParent);
            CHECK_FALSE(colorAllele2NotFromCorrectParent);
        }
        
        THEN("Alleles are selected randomly from each parent"){
        CHECK(inheritedMotherAllele1 != SAMPLE_SIZE); // check that there is variation when selecting between maternal allele.
        CHECK(inheritedFatherAllele1 != SAMPLE_SIZE);
            
        }
    }
}

	
