n = GetN()

if n <= 1 then
	result = 1
else
	f = GetFightData(n - 1)
	m, y = GetRecordData(n - 1)

	if f == 1 then
		result = (m + 1) % 3
	elseif f == 0 then
		result = (y + 2) % 3
	elseif f == -1 then
		result = y
	end

	ra = 0.0
	if n > 10 then
		for i=n, n-10, -1 do
			ra = ra + GetFightData(i)
		end
		
		ra = ra / (n + 1)
		if ra <= 0.5 then
			result = (result + 2) % 3
		end
	end
end