NE PAS ENLEVER LE FOLDER SRC: Arduino n'autorise de sous dossier ssi il est sous le nom src 


###########PRISE DU STACK###############
0. vérifier sa position avec les infrarouges
1. activer 4 aimants
2. lever de quelques millimètres
3. descendre les doigts
-- envoyer signal ok bouger

#########SEPARER LE STACK############
-- recevoir signal ok position
1. désactiver les aimants extérieurs
(les cannettes ext vont tomber)
2. avancer ses cremaillères (attention synchro raspberry)
3. infra rouges capte planche peut tomber
4. remonter les doigts de 90 degrés
5. reculer cremaillères vite  - monter les canettes accrochées lentement et lancement avant
-- attendre le signal de la raspberry (savoir si elle a suffisamment avancé)
5. descendre les cannettes accrochées
6. désactiver les aimants

##############PINOUT################
FING1 : 9
FING2 : 10

IR_1 : 25
IR_2 : 24

CAN1 : 28
CAN2 : 29
CAN3 : 33
CAN4 : 36

CREM1 : DIR = 41
	STEP = 14
	MS3 = 15
	MS2 = 16
	MS1 = 17
	EN = 20

LIFT :  DIR = 34
	STEP = 35
	MS3 = 37
	MS2 = 38
	MS1 = 39
	EN = 40

	Microswitch = 27 