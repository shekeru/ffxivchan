#pragma once

enum Job {
	Adventurer = 0,
	// Classes
	Gladiator = 1,
	Pugilist = 2,
	Marauder = 3,
	Lancer = 4,
	Archer = 5,
	Conjurer = 6,
	Thaumaturge = 7,
	// DoH
	Carpenter = 8,
	Blacksmith = 9,
	Armorer = 10,
	Goldsmith = 11,
	Leatherworker = 12,
	Weaver = 13,
	Alchemist = 14,
	Culinarian = 15,
	// DoL
	Miner = 16,
	Botanist = 17,
	Fisher = 18,
	// Jobs
	Paladin = 19,
	Monk = 20,
	Warrior = 21,
	Dragoon = 22,
	Bard = 23,
	White_Mage = 24,
	Black_Mage = 25,
	// Expansions
	Arcanist = 26,
	Summoner = 27,
	Scholar = 28,
	Rogue = 29,
	Ninja = 30,
	// Heavensward
	Machinist = 31,
	Dark_Knight = 32,
	Astrologian = 33,
	// Stormblood
	Samurai = 34,
	Red_Mage = 35,
	Blue_Mage = 36,
	// Shadowbringers
	Gunbreaker = 37,
	Dancer = 38,
};

namespace Action {
	enum Action {
		//GLA
		Fast_Blade = 9,
		Riot_Blade = 15,
		Rage_of_Halon = 21,
		//MRD
		Heavy_Swing = 31,
		Maim = 37,
		Overpower = 41,
		Storms_Path = 42,
		// LNC
		True_Thrust = 75,
		Vorpal_Thrust = 78,
		Full_Thrust = 84,
		Disembowel = 87,
		Chaos_Thrust = 88,
		// RNG
		Heavy_Shot = 97,
		Straight_Shot = 98,
		// ROG
		Spinning_Edge = 2240,
		Shade_Shift = 2241,
		Gust_Slash = 2242,
		// SAM
		Hakaze = 7477,
		Jinpu = 7478,
		Shifu = 7479,
		Yukikaze = 7480,
		Gekko = 7481,
		Kasha = 7482,
		Fuga = 7483,
		Mangetsu = 7484,
		Oka = 7485,
		// RDM
		Jolt = 7503,
		Riposte = 7504,
		Verthunder = 7505,
		Corps_Corps = 7506,
		Veraero = 7507,
		Verfire = 7510,
		Verstone = 7511,
		Zwerchhau = 7512,
		Redoublement = 7516,
		Verthunder_II = 16524,
		Veraero_II = 16525,
	};
};

namespace Status {
	enum Status {
		// LNC
		Life_Surge = 116,
		Chaos_Thrust = 118,
		Power_Surge = 120,
		// RNG
		StraightShotReady = 122,
		// Caster
		Swiftcast = 167,
		VerfireReady = 1234,
		VerstoneReady = 1235,
		Dualcast = 1249,
		// Samurai
		Jinpu = 1298,
		Shifu = 1299,
		// Changes
		Disembowel = 1914,
	};
};