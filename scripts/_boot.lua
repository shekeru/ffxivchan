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
  local metas, insts = {}, {}
  for k, v in pairs(_G) do
    if k:match("^[A-Z]%a+:%d+$") then
      insts[k] = v
    elseif k:match("^[A-Z]%a+$") then
      metas[k] = v
  end end
  print("==== Class Tables ====")
  xiv.show(metas); print("\n")
  print("====== Objects ======")
  xiv.show(insts); print("\n")
end
