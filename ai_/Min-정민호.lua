n = GetN()
k = n % 3

if k == 0 then
result = 1
elseif k == 1 then
result = GetRandom() % 3
else
result = GetRandom() % 2
end 