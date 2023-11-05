TLE = 20*60

graphs =              ["brock200", "diseasome", "email EU", "retweet", "spam", "bio-dmela", "amazon", "occupywallstnyc", "wikipedia2009", "youtube"]

graphs = ["brk2", "dis", "email", "RT", "spam", "dmela", "amaz", "OWS", "wiki", "YT"]


edgeswap_lookupG = [2.7,         0.014,        52.4,      11.82,      1.57,   2.787,      850,       0.0123,             TLE,            TLE]
lookupG =          [1.4,          0.004,      1.44,       3.38,       0.24,   0.1,            11,         0.004,          TLE,            TLE]
lookupG_no_X =     [5.9,         0.0046,      1.68,       7.134,     1.466,   0.131,     11.12,        0.0053,            TLE,            TLE]
no_X =                [13.6,         0.0017,      0.601,      24.38,     2.49,    0.050,     1.41,       0.013,              27.44,          228.33]
els_set =             [7.17,        0.0014,     0.57,       11.12,      1.35,    0.04,      0.137,      0.012,              21.16,          134.25]
edgeswap =            [4.22,        0.295,      53,         5.2,        5.37,     5.26,     385.092,      0.0064,           TLE,             TLE]
edgeswap_save =       [3.98,        0.019,      53.9,         10,         5.4,      5.3,      405.9,          0.015,           TLE,            TLE]
edgeswap_P_only =     [4.45,        0.03,       52.74,     16,         5.39,    5.27,      407.01,         0.018,          TLE,             TLE]
edgeswap_XP =         [2.56,        0.005,      0.28,       7.08,       0.35,     0.042,    0.151,      0.011,              14.25,          35.37]
naive_hashmap =       [55.8,        0.012,      4.74,       98.85,      10.8,    0.33,      0.652,      0.037,              145.96,         TLE]
HlookupG =              [5.08,      0.003,       0.28,       42.1,       0.92,    0.093228,  0.32,       0.0198,             75.83,          26.34]
HlookupG_no_X  =         [9.71,      0.0042,      0.933,     46.07,      2.22,    0.15,      0.38,       0.019,            75.83,            158.83]

impl_ordering = ["edgeswap_lookupG", "lookupG","lookupG_no_X","no_X", "els_set", "edgeswap", "edgeswap_save", "edgeswap_P_only", "edgeswap_XP", "H_naive", "HlookupG", "HlookupG_no_X"]
impl = {"edgeswap_lookupG":edgeswap_lookupG, "lookupG": lookupG, "lookupG_no_X": lookupG_no_X, "no_X":no_X, "els_set":els_set, "edgeswap": edgeswap, "edgeswap_save": edgeswap_save, "edgeswap_P_only": edgeswap_P_only, "edgeswap_XP": edgeswap_XP, "H_naive": naive_hashmap, "HlookupG": HlookupG, "HlookupG_no_X": HlookupG_no_X}



def nf(i):
    return i.replace("_", "\\_")

def get_column_map(a):
    res = {}
    for i in impl:
        res[i] = impl[i][a]
    return res

def get_column(a):
    res = []
    for i in impl_ordering:
        res.append(impl[i][a])
    return res

def get_entry(i, g):
    if type(g) == str:
        return impl[i][graphs.index(g)]
    elif type(g) == int:
        return impl[i][g]
    else:
        assert False

def place_TLE(row):
    res = []
    for i in row:
        if i == TLE:
            res.append("TLE")
        else:
            res.append(i)
    return res


def print_results():
    for i in impl_ordering:
        row = " & ".join(map(str,place_TLE(impl[i])))
        print(f"{nf(i)} & {row} \\\\")
    print("\n\n")
    

def print_average_rankings():
    ranked = []
    for i,g in enumerate(graphs):
        ranked.append(sorted(get_column(i)))

    for i in impl_ordering:
        rank_sum = 0
        for gi, g in enumerate(graphs):
            e = get_entry(i,g)
            rank_sum += ranked[gi].index(e) + 1
        print(f"{nf(i)} & {rank_sum/len(graphs)} \\\\")
    print("\n\n")
            
        
    
def print_header():
    print("impl &"," & ".join(graphs), "\\\\\\hline")

def print_diff_from_best():
    best = []
    for i,g in enumerate(graphs):
        best.append(min(get_column(i)))

        
    for i in impl_ordering:
        row = []
        for gi, gt in enumerate(impl[i]):
            if gt == TLE:
                row.append("N/A")
            else:
                row.append(round(gt/best[gi],2))
            
        row = " & ".join(map(str,row))
        print(f"{nf(i)} & {row} \\\\")
    print("\n\n")


def print_average_diff():
    best = []
    for i,g in enumerate(graphs):
        best.append(min(get_column(i)))

        
    for i in impl_ordering:
        row = []
        for gi, gt in enumerate(impl[i]):
            if gt == TLE:
                pass
            else:
                row.append(gt/best[gi])
            
        row = round(sum(row)/len(row),2)
        print(f"{nf(i)} & {row} \\\\")
    print("\n\n")    

print_header()
print_results()
print_average_rankings()
print_diff_from_best()


print_average_diff()
