[top]
components : ro@RouterOutput
out : out
in : from_RPU interfaceNum
link : out@ro out
link : interfaceNum interfaceNum@ro
link : from_RPU from_RPU@ro

[ro]
preparation : 00:00:00:050