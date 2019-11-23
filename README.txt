	A. Présentation du programme:

Notre programme permet de simuler l'évolution d’un environnement contenant divers types d’entités (plantes et animaux) en fonction des différentes saisons (été, automne, hiver et printemps).
Il existe deux types d’animaux:
- Les loups:
	Ils sont capables soit de se déplacer en meute soit de se reproduire, s’alimenter, boire ou se reposer.
	Ils ne mangent que des moutons, et ne peuvent boire que de l’eau.
- Les moutons:
	Ils sont capables soit de se déplacer en troupeau, soit de se reproduire, s’alimenter, boire ou se reposer.
	Contrairement aux loups, ils peuvent se nourrir de différentes entités : 
	les massifs d’herbes, les fleurs et les champignons “hallucinogènes”. 
	Ils peuvent bien-sûr aussi boire de l’eau.

Le programme modélise également trois types d’entités consommables qui ne sont pas des animaux:
- Les flaques d’eau:
	Elles permettent de diminuer la soif des moutons ainsi que celle des loups. 
	Elles apparaissent en automne et au printemps quand la simulation 
	est en mode apparition automatique des entités consommables (AADEC). 
	De plus leur taille diminue en été et elles grandissent deux fois plus vite en automne.
- les massifs d’herbe: 
	Ils permettent uniquement d’augmenter le niveau d’énergie des moutons quand ils n’en ont plus assez. 
	Ils apparaissent uniquement au printemps quand la simulation est en mode AADEC. 
	De plus ils ne grandissent pas en hiver et grandissent deux fois plus vite au printemps.
- les fleurs: 
 	Elles permettent comme les massifs d’herbe d’augmenter le niveau d’énergie des moutons 
	quand il n’en ont plus assez, mais ils permettent aussi de régénérer leur santé 
	(qui peut être affectée par la présence de virus).
	Elles apparaissent uniquement au printemps quand la simulation est en mode AADEC. 
	De plus elles ne grandissent pas en hiver et grandissent deux fois plus vite au printemps.
- les champignons hallucinogènes: 
 	L’effet d’augmentation du niveau d’énergie des moutons est négligeable du fait de leur petite taille, 
	leur principal effet est de causer des hallucinations aux moutons.
	Pendant un court temps après leur ingestion, les moutons sont désorientés et ne parviennent pas 
	à avoir un comportement normal et à se diriger peu importe ce qui les entoure.
	Ils apparaissent uniquement en automne quand la simulation est en mode AADEC.
	De plus ils ne grandissent pas peu importe la saison.

Le programme modélise également les infections des animaux par des virus. Chaque animal possède un système immunitaire pour se défendre contre ces infections.

Nous pouvons suivre la quantité de tous les types d’entités présentes (loups-moutons-eau-verdure, on ne distingue pas l’herbe, les fleurs et les champignons) sur des courbes d'évolution et des compteurs.
Nous pouvons nous focaliser sur certaines entités pour afficher les courbes correspondant à son état (niveau d’énergie pour les entités consommables et des informations plus précises comme l’état du système immunitaire pour les animaux).

Au niveau de l’environnement, il existe plusieurs états :
Le mode par défaut dans lequel des entités consommables par les moutons ou les loups apparaissent automatiquement en fonction des saisons. Il est le plus représentatif de la réalité, il serait utilisé pour montrer ce programme à des personnes qui n’ont aucun besoin d’avoir des informations précises sur les entités et qui souhaiterait juste effectuer une simulation. Ils auraient seulement à ajouter les loups et les moutons.
Le mode debug dans lequel on voit les champs de vision des entités qui en ont un, des informations supplémentaires sur l’état des animaux ainsi que leur représentation au niveau du programme (des cercles).
Le mode apparition automatique des entités consommables peut être désactivé pour permettre à l’utilisateur de mettre les entités dont il a envie pour faire différents scénarios précis.  

———————————————————————————————————————————————————————————————————————————————————————

	B. Compilation du programme:

Pour compiler/exécuter le programme général, il est nécessaire de lancer la commande:

			"scons application-run –-cfg=app.json"

(Il est aussi possible de le lancer avec la commande "scons application-run" car c’est le fichier app.json qui est pris par défaut) depuis le répertoire contenant SConscript.

———————————————————————————————————————————————————————————————————————————————————————
	C. Utilisation du programme :

Pour le programme général (à lancer avec app.json):
	
Ajout d’entité :
	1. Loups simples (appartient par défaut à la meute 1) :
		Placer la souris dans l’environnement et appuyer sur "W" sur le clavier.

	2. Moutons simples (appartient par défaut au troupeau 1):
		Placer la souris dans l’environnement et appuyer sur "S" sur le clavier.

	3. Massifs d’herbe :
		Placer la souris dans l’environnement et appuyer sur "G" sur le clavier.

	4. Fleurs régénératrices:
		Placer la souris dans l’environnement et appuyer sur "F" sur le clavier.

	5. Champignons hallucinogènes :
		Placer la souris dans l’environnement et appuyer sur "M" sur le clavier.

	6. Flaques d’eau:
		Placer la souris dans l’environnement et appuyer sur "E" sur le clavier.

	7. Moutons appartenant au troupeau 1 (même troupeau que celui par défaut avec "S"):
		Placer la souris dans l’environnement et appuyer sur "1" sur le clavier.

	8. Moutons appartenant au troupeau 2:
		Placer la souris dans l’environnement et appuyer sur "2" sur le clavier.

	9. Moutons appartenant au troupeau 3:
		Placer la souris dans l’environnement et appuyer sur "3" sur le clavier.

	10. Loups appartenant à la meute 1 (même meute que celle par défaut avec "W"):
		Placer la souris dans l’environnement et appuyer sur "F1" sur le clavier.

	11. Loups appartenant à la meute 2:
		Placer la souris dans l’environnement et appuyer sur "F2" sur le clavier.

	12. Loups appartenant à la meute 3:
		Placer la souris dans l’environnement et appuyer sur "F3" sur le clavier.

Actions sur une entité :
	1. Infection d’une entité:
		Placer la souris sur une entité et appuyer sur "I" sur le clavier pour infecter l’entité pointée.

	2.Tuer une entité :
		Placer la souris sur une entité et appuyer sur "K" sur le clavier pour tuer l’entité pointée.

		
Autres entrées clavier :
	Passage au graphe général : 
	Appuyer sur "Z" pour passer au graphique général affichant le nombre d’entité de chaque type.

	Reset : 
	Appuyer sur "R" pour recommencer la simulation.

	Debug : 
	Appuyer sur "D" pour activer/désactiver le mode debug.

	Génération automatique d’entités (AADEC): 
	Appuyer sur la touche "A" pour activer/désactiver le mode apparition automatique des entités consommables. 
	
