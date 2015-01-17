n = GetN()

if n == 0 then
	result = GetRandom()
elseif n < 100 then
	result = n % 3
	result = 2 - result
else
	r = GetData(0)
	if r == 0 then
		m = 0
		for i = 0, n - 1 do
			m = m + GetFightData(i)
		end

		SaveData(0, m)

	elseif r == 2 then
		result = n % 3
		result = 2 - result
	else
		result = (n + 1) % 3
	end
end
