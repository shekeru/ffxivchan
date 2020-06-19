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
		Holmgang = 43,
		Storms_Eye = 45,
		Inner_Beast = 49,
		Steel_Cyclone = 51,
		// PGL
		Bootshine = 53,
		True_Strike = 54,
		Snap_Punch = 56,
		Twin_Snakes = 61,
		Arm_of_Destroyer = 62,
		Demolish = 66,
		Rockbreaker = 70,
		Fourpoint_Fury = 16473,
		// LNC
		True_Thrust = 75,
		Vorpal_Thrust = 78,
		Life_Surge = 83,
		Full_Thrust = 84,
		Lance_Charge = 85,
		Doom_Spike = 86,
		Disembowel = 87,
		Chaos_Thrust = 88,
		// RNG
		Heavy_Shot = 97,
		Straight_Shot = 98,
		Venomous_Bite = 100,
		Windbite = 113,
		// CNJ
		Stone = 119,
		Cure = 120,
		Aero = 121,
		Medica = 124,
		Raise = 125,
		//Esuna = 126,
		MedicaII = 133,
		CureII = 135,
		FluidAura = 134,
		Regen = 137,
		// BLM
		Fire = 141,
		Blizzard = 142,
		Thunder = 144,
		Blizzard_II = 146,
		Fire_II = 147,
		Thunder_II = 148,
		Fire_III = 152,
		Blizzard_III = 154,
		// ARC
		Ruin = 163,
		Bio = 164,
		Energy_Drain = 16508,
		Miasma = 168,
		Resurrection = 173,
		Fester = 181,
		// WAR
		Mythril_Tempest = 16462,
		// ROG
		Spinning_Edge = 2240,
		Shade_Shift = 2241,
		Gust_Slash = 2242,
		Aeolian_Edge = 2255,
		// MCH
		SplitShot = 2866,
		SlugShot = 2868,
		HotShot = 2872,
		CleanShot = 2873,
		Reassemble = 2876,
		// AST
		Draw = 3590,
		Redraw = 3593,
		Benefic = 3594,
		Aspected_Benefic = 3595,
		Malefic = 3596,
		Combust = 3599,
		Helios = 3600,
		Aspected_Helios = 3601,
		Ascend = 3603,
		Diurnal_Sect = 3604,
		Nocturnal_Sect = 3605,
		// DRK
		Hard_Slash = 3617,
		Syphon_Strike = 3623,
		Souleater = 3632,
		// SB Skills
		Sonic_Thrust = 7397,
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
		Enpi = 7486,
		// RDM
		Jolt = 7503,
		Riposte = 7504,
		Verthunder = 7505,
		Corps_Corps = 7506,
		Veraero = 7507,
		Scatter = 7509,
		Verfire = 7510,
		Verstone = 7511,
		Zwerchhau = 7512,
		Vercure = 7514,
		Redoublement = 7516,
		Verraise = 7523,
		Verthunder_II = 16524,
		Veraero_II = 16525,
		// Role Shit
		Esuna = 7568,
		Rescue = 7571,
		// DNC
		Cascade = 15989,
		Fountain = 15990,
		Reverse_Cascade = 15991,
		Fountainfall,
		Windmill,
		Bladeshower,
		Rising_Windmill,
		Bloodshower,
		Standard_Step = 15997,
		Technical_Step = 15998,
		Emboite = 15999,
		Entrechat,
		Jete = 16001,
		Pirouette = 16002,
		Closed_Position = 16006,
		// GNB
		Keen_Edge = 16137,
		Brutal_Shell = 16139,
		Demon_Slice = 16141,
		Danger_Zone = 16144,
		Solid_Barrel = 16145,
		Gnashing_Fang = 16146,
		Savage_Claw = 16147,
		Demon_Slaughter = 16149,
		Wicked_Talon = 16150,
		Burst_Strike = 16162,
		// WHM
		Afflatus_Solace = 16531,
		// Changes
		Total_Eclipse = 7381,
		MinorArcana = 7443,
		Prominence = 16457,
		Repose = 16560,
		Play = 17055,
		// SCH why
		Bio_SCH = 17864,
		Ruin_SCH = 17869,
	};
};

namespace Status {
	enum Status {
		// MRD
		Storms_Eye = 90,
		// PGL
		Twin_Snakes = 101,
		Opo_Form = 107,
		Raptor_Form = 108,
		Coeurl_Form = 109,
		// LNC
		Life_Surge = 116,
		Chaos_Thrust = 118,
		Power_Surge = 120,
		// RNG
		StraightShotReady = 122,
		VenomousBite = 124,
		Windbite = 129,
		// CNJ
		Aero = 143,
		AeroII = 144,
		MedicaII = 150,
		Freecure = 155,
		Regen = 158,
		// BLM
		Firestarter = 165,
		Swiftcast = 167,
		// SMN
		Bio = 179,
		Miasma = 180,
		Miasma_II  = 188,
		Bio_II  = 189,
		// AST
		Aspected_Benefic = 835,
		Aspected_Helios = 836,
		Combust = 838,
		Dinural_Sect = 839,
		Nocturnal_Sect = 840,
		CombustII = 843,
		// RDM?
		VerfireReady = 1234,
		VerstoneReady = 1235,
		Dualcast = 1249,
		// Samurai
		Jinpu = 1298,
		Shifu = 1299,
		// DNC
		FlourishingCascade = 1814,
		FlourishingFountain = 1815,
		FlourishingWindmill = 1816,
		FlourishingShower = 1817,
		// Changes
		Disembowel = 1914,
	};
};

namespace CombatFlags {
	enum Mask : unsigned char {
		None = '\0',
		IsAggressive = 1 << 0,
		InCombat = 1 << 1,
		WeaponUnsheathed = 1 << 2,
		_UNKNOWN_8_POS = 1 << 3,
		PartyMember = 1 << 4,
		AllianceMember = 1 << 5,
		Friend = 1 << 6,
		IsCasting = 1 << 7
	};
};

namespace EntityType {
	enum Type : unsigned char {
		Unknown = '\0',
		Player = 1 << 0,
		Monster = 1 << 1,
		NonPlayer = 1 << 2,
	};
}