local file = io.open(CONFIG, "r")

local columns = {}

if file then
  for line in file:lines() do
    for value in (line .. ","):gmatch("([^,]*),") do
      table.insert(columns, value)
    end
  end

  file:close()
end

return columns
