n = GetN()

result = n % 3

if (n > 1) then

	mm, kk = GetRecordData(n - 1)

	for i=n-2,0,-1 do
		m, k = GetRecordData(i)		
		if (m == mm and k == kk) then
			m,k = GetRecordData(i+1)
			result = (k+1)%3
			break
		end
	end

end