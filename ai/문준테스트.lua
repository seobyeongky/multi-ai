n=GetN()
a=0
if n==0 then
result=GetRandom()%3
for i=1,100 do
	a=GetRandom()%3+2
	SaveData(10*i,a)
	SaveData(10*i+1,a+2)
	SaveData(10*i+2,a*a+5)
	SaveData(10*i+3,a*a*a)
	SaveData(10*i+4,1)
	SaveData(10*i+5,a+3)
	SaveData(10*i+6,0)
	SaveData(10*i+7,2)
	SaveData(10*i+8,a)
	SaveData(10*i+9,a+1)
end
else
result=GetData(n)%3
end