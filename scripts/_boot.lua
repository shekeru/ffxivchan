-- Fuck you Enix
setmetatable(_G, {
  __newindex = function (t, n, v)
    rawset(t, n, v)
  end,
  __index = function (t, n)
    return nil
  end,
}); xiv = {}
-- Helper Functions
function show(obj)
	for k, v in pairs(obj) do
		print(" [+]", k, "->", v)
end end
-- Fuck Me More
function xiv.state()
  xiv.metas, xiv.insts = {}, {}
  for k, v in pairs(_G) do
    if k:match("^%u%w+:%x+$") then
      xiv.insts[k] = v
    elseif k:match("^%u%w+$") then
      xiv.metas[k] = v
  end end
  print("==== Class Tables ====")
  xiv.show(xiv.metas); print("\n")
  print("====== Objects ======")
  xiv.show(xiv.insts); print("\n")
end
-- Player Listing
function xiv.players()
  local t = {}
  for k,v in pairs(_G) do
    if k:match("^Pc:%w+$") then
      t[#t+1] = v; print(k)
  end end; return t end
-- Retainer's Bell
function xiv.bell()
  for k, v in pairs(_G) do
    if k:match("^CmnDefRetainerBell:%x+$") then
      return v:OpenRetainerList()
  end end end
