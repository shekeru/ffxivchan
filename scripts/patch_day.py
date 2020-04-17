import idc
# Direct Functions
def Location(Name, Signature, offset = 0):
    addr = idc.FindBinary(0, idc.SEARCH_DOWN, Signature)
    if addr > 1:
        idc.MakeName(addr + offset, Name)
    else:
        print("Error on:", Name)
# Indirect By Call
def Function(Name, Signature, offset = 0, arg = 0):
    addr = idc.FindBinary(0, idc.SEARCH_DOWN, Signature)
    follow = idc.GetOperandValue(addr + offset, arg);
    if follow > 1:
        idc.MakeName(follow, Name)
    else:
        print("Error on:", Name, hex(follow))
# Zoom Manager
Location("UpdateTick", "40 55 53 48 8d 6c 24 b1 48 81 ec c8 00 00 00 48 8b d9")
# Action Manager
Function("GetIcon", "E8 ? ? ? ? F6 DB 8B C8")
Location("IsIconReplaceable", "81 f9 2e 01 00 00 7f 39 81 f9")
Location("RequestAction", "40 53 55 57 41 54 41 57 48 83 ec 60")
Function("AllowRequestsGCD", "48 8b cf e8 ? ? ? ? 84 c0 74 31", 3)
Function("GetRecastIndex", "e8 ? ? ? ? 83 f8 39 48")
Location("GetRecastStruct", "40 53 48 83 ec 20 48 63 da 85 d2 78")
# Globals
Function("LocalActor", "48 8b 05 ? ? ? ? 48 89 6c 24 60", 0, 1)
Function("JobHud", "48 8d 0d ? ? ? ? 0f 95 c2 e8", 0, 1)
Function("ComboSys", "48 89 2D ? ? ? ? 85 c0")
# Rapture Section
Function("SendAction", "e8 ? ? ? ? 8b 44 24 20 c1 e8 05")
Function("SpawnWindow", "e8 ? ? ? ? 85 c0 75 1d 48 8b 0b")
Location("CtxSets_R5", "4c 89 4c 24 20 44 89 44 24 18 53 55 56 57 48 81")
Function("WindowReady", "48 8b ce e8 ? ? ? ? 48 8b 4d 4f", 3)
