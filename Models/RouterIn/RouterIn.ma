[top]
components : ri@RouterIn
out : to_RPU flag interfaceNum
in : in ready
link : to_RPU@ri to_RPU
link : interfaceNum@ri interfaceNum
link : flag@ri flag
link : in in@ri
link : ready ready@ri

[ri]
preparation : 00:00:00:010