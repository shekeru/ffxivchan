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
function xiv.show(obj)
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
  for k,v in pairs(xiv.insts) do
    if k:match("^Pc:%w+$") then
      print(k)
end end end
