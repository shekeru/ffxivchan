#pragma once

namespace Job {
	enum JobID : BYTE {
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
		Reaper = 39,
		Sage = 40,
	};
};

namespace Action {
	enum ActionType : BYTE {
		None = 0x00,
		Spell = 0x01,
		Item = 0x02,
		Key_Item = 0x03,
		Ability = 0x04,
		General = 0x05,
		Companion = 0x06,
		Unk_7 = 0x07,
		Unk_8 = 0x08, //something with Leve?
		Craft_Action = 0x09,
		Main_Command = 0x0A,
		Pet_Action = 0x0B,
		Unk_12 = 0x0C,
		Mount = 0x0D,
		PvP_Action = 0x0E,
		Waymark = 0x0F,
		Chocobo_Race_Ability = 0x10,
		Chocobo_Race_Item = 0x11,
		Unk_18 = 0x12,
		Squadron_Action = 0x13,
		Accessory = 0x14
	};

	enum ActionID : USHORT{
		// GLA
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
		Infuriate = 52,
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
		Transpose = 149,
		Fire_III = 152,
		Blizzard_III = 154,
		Freeze = 159,
		Flare = 162,
		// ARC + SMN
		Ruin = 163,
		Aetherflow = 166,
		Energy_Drain_SCH = 167,
		Resurrection = 173,
		Fester = 181,
		Painflare = 3578,
		Deathflare = 3582,
		Energy_Drain_SMN = 16508,
		Energy_Syphon = 16510,
		Gemshine = 25883,
		// WAR
		Mythril_Tempest = 16462,
		// ROG
		Spinning_Edge = 2240,
		Shade_Shift = 2241,
		Gust_Slash = 2242,
		Death_Blossom = 2254,
		Aeolian_Edge = 2255,
		// MCH
		Split_Shot = 2866,
		Slug_Shot = 2868,
		Hot_Shot = 2872,
		Clean_Shot = 2873,
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
		Unleash = 3621,
		Syphon_Strike = 3623,
		Souleater = 3632,
		// Stormblood Skills
		Total_Eclipse = 7381,
		Sonic_Thrust = 7397,
		Heat_Blast = 7410,
		Minor_Arcana = 7443,
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
		Addle = 7560,
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
		Continuation = 16155,
		Burst_Strike = 16162,
		// Updated Changes
		Stalwart_Soul = 16468,
		Hakke_Mujinsatsu = 16488,
		Afflatus_Solace = 16531,
		Prominence = 16457,
		Repose = 16560,
		Play = 17055,
		// SCH why
		Bio_SCH = 17864,
		Ruin_SCH = 17869,
		// Updates
		Sleep = 25880,
	};
};

namespace StatusFlags {
	enum Mask : unsigned char {
		None = '\0',
		Aggressive = 1 << 0,
		InCombat = 1 << 1,
		WeaponUnsheathed = 1 << 2,
		_UNKNOWN_8_POS = 1 << 3,
		_UNKNOWN_16_POS = 1 << 4,
		PartyMember = 1 << 5,
		AllianceMember = 1 << 6,
		IsFriend = 1 << 7,
		// IsCasting = 1 << 8
	};
};


// Self: 01 + 04 (100)
// Monter: 02 + 05 (101)
// Thancred: 02 + 09 (1001)
namespace EntityType {
	enum Type : unsigned char {
		Unknown = '\0',
		Player = 1 << 0,
		Monster = 1 << 1,
		NonPlayer = 1 << 2,
	};
}