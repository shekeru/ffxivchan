import ctypes, idc

class ReadPattern:
    def __init__(s, Name, Raw):
        Signature, s.Et, s.Arr = "", 0, Raw.lower().split()
        while s.Et < len(s.Arr) and s.Arr[s.Et] not in dir(s):
            Signature += s.Arr[s.Et] + " "; s.Et += 1
        s.Addr = idc.FindBinary(0, idc.SEARCH_DOWN, Signature)
        while s.Et < len(s.Arr):
            if 'read' == s.Arr[s.Et]:
                return s.read(Name)
            getattr(s, s.Arr[s.Et])(); s.Et += 1
        print Name, '->' , hex(s.Addr)
        idc.MakeName(s.Addr, Name)
    def add(s):
        s.Et += 1; s.Addr += int(s.Arr[s.Et])
    def sub(s):
        s.Et += 1; s.Addr -= int(s.Arr[s.Et])
    def tracerelative(s):
        s.Addr += ctypes.c_int32(idc.Dword(s.Addr)).value + 4
    def tracecall(s):
        s.Addr += 1; s.tracerelative()
    def read(s, Name):
        print "Offset:", Name, hex(s.Addr)
        idc.MakeComm(s.Addr, Name)

# Globals
ReadPattern("LocalActor",
    "48 8b 05 ?? ?? ?? ?? 48 89 6c 24 60 Add 3 TraceRelative")

# Actions
ReadPattern('ActionManager',
        "48 8d 0d ?? ?? ?? ?? 89 44 ?? ?? ?? ff c0 89 44 ?? ?? ?? b9 ?? ?? ?? ?? 89 44 Add 3 TraceRelative")
ReadPattern("RequestAction",
    "?? 53 55 57 ?? 54 ?? 57 ?? 83 ec ?? 83 bc ?? ?? ?? ?? ?? ?? ?? 8b e9 ?? 8b e0 ?? 8b fa ?? 8b f9 ?? 8b d8 74 ?? 80 79 68 ?? 74")
ReadPattern("InteractParam",
    "48 8D 0D ?? ?? ?? ?? 48 83 C4 ?? E9 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 83 EC ?? 48 8B 0D ?? ?? ?? ?? Add 3 TraceRelative")
ReadPattern("InteractWith",
    "48 89 5C 24 ?? 57 48 83 EC ?? F6 81 ?? ?? ?? ?? ?? 48 8B DA 48 8B F9 0F 85 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ??")
ReadPattern("IsIconReplaceable",
    "81 f9 2e 01 00 00 7f 39 81 f9")
ReadPattern("GetIcon",
    "e8 ? ? ? ? 44 8b f0 8b d8 TraceCall")

# ActorController
ReadPattern("ActorList",
    "48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B D8 48 8B D3 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8D 8E Add 3 TraceRelative")
ReadPattern("TargetPointer",
    "48 83 3D ?? ?? ?? ?? ?? 75 ?? C7 43 Add 3 TraceRelative Add 1")

# Todo: Organize
ReadPattern("InterfaceManager",
    "e8 ?? ?? ?? ?? 48 85 c0 74 3c 48 8b 10 Sub 4 TraceRelative")
ReadPattern("SafelyGetUiModule",
    "e8 ?? ?? ?? ?? 48 85 c0 74 3c 48 8b 10 TraceCall")
ReadPattern("GetRecastIndex",
    "0f 29 7c 24 20 e8 ?? ?? ?? ?? 8b d0 Add 6 TraceRelative")
ReadPattern("GetRecastStruct",
    "40 53 48 83 ec 20 48 63 da 85 d2 78")

ReadPattern("ComboArea",
    "48 89 2D ?? ?? ?? ?? 85 c0 Add 3 TraceRelative")
ReadPattern("JobHud",
    "ff 48 8d 0d ?? ?? ?? ?? 0f 95 c2 e8 Add 4 TraceRelative")

# VMTs
#ReadPattern("UiModuleVMT",
    #"48 8d 05 ? ? ? ? 48 89 71 28 4c 8b e2 Add 3 TraceRelative")

# CSV File
ReadPattern("NetworkRequest",
    "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 8B E9 41 8B D9 48 8B 0D ? ? ? ? 41 8B F8 8B F2")

# Comments?
ReadPattern("ActorList_MaxLen",
    "81 FA ?? ?? ?? ?? 77 ?? 53 Read")
