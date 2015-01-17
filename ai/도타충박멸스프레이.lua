z = 2000
n = GetN()
N = n-1

result = -1

if 0 < n then

	my, en = GetRecordData(N)
	
	SaveData(0 * z + N, my)
	SaveData(1 * z + N, en)
	SaveData(2 * z + N, GetFightData(N))	
	
	
end


if n < 5 then
	result = GetRandom()%3
elseif 5 <= n then
	k = 0					--sizeof pattern	k
	while k < N do
		k= k+1
		for j = 0 , N - k*2  do		--starting point	j
			match = true
			for t = 0, k do		--matching temp
				if GetData(1*z + j+t) ~= GetData(1*z + j + k + t) then
					match = false
				end
			end
			if  match then
				result = GetData(1*z + j+k+1)+1
				result = result % 3
			end
		end
	end
		
end

if(result = -1)
	result = en
end
	





























