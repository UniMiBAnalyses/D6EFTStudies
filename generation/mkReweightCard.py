from itertools import combinations
import argparse
import os
import sys 

if __name__ == "__main__":
    
    parser = argparse.ArgumentParser()
    parser.add_argument("-op","--op", help="Operator list to make the reweight card follwoing algebra" , nargs="+", required=True)
    parser.add_argument("-o", "--out", help="Out file name", required=True)
    parser.add_argument("-cr", "--change_model", help="Add a change model line to add new restriction at each reweight point", required=False, action="store_true")
    parser.add_argument("-dop", "--default_operators", nargs = "+", help="Comma separated list of operators that are turned on in the proc card. Will be set to zero in the reweight cards for SM", required=False, default="")
    args = parser.parse_args()

    full_ops = {
        'cG': 1,
        'cW': 2,
        'cH': 3,
        'cHbox': 4,
        'cHDD': 5,
        'cHG': 6,
        'cHW': 7,
        'cHB': 8,
        'cHWB': 9,
        'ceHRe': 10,
        'cuHRe': 11,
        'cdHRe': 12,
        'ceWRe': 13,
        'ceBRe': 14,
        'cuGRe': 15,
        'cuWRe': 16,
        'cuBRe': 17,
        'cdGRe': 18,
        'cdWRe': 19,
        'cdBRe': 20,
        'cHl1': 21,
        'cHl3': 22,
        'cHe': 23,
        'cHq1': 24,
        'cHq3': 25,
        'cHu': 26,
        'cHd': 27,
        'cHudRe': 28,
        'cll': 29,
        'cll1': 30,
        'cqq1': 31,
        'cqq11': 32,
        'cqq3': 33,
        'cqq31': 34,
        'clq1': 35,
        'clq3': 36,
        'cee': 37,
        'cuu': 38,
        'cuu1': 39,
        'cdd': 40,
        'cdd1': 41,
        'ceu': 42,
        'ced': 43,
        'cud1': 44,
        'cud8': 45,
        'cle': 46,
        'clu': 47,
        'cld': 48,
        'cqe': 49,
        'cqu1': 50,
        'cqu8': 51,
        'cqd1': 52,
        'cqd8': 53,
        'cledqRe': 54,
        'cquqd1Re': 55,
        'cquqd11Re': 56,
        'cquqd8Re': 57,
        'cquqd81Re': 58,
        'clequ1Re': 59,
        'clequ3Re': 60
    }
    
    # begin
    print("---> Start")
    
    # dop = args.default_operators.split(",")
    dop = args.default_operators

    f = open(args.out, "w")
    
    f.write("change helicity False\n")
    f.write("change rwgt_dir rwgt\n\n")
    #SM
    f.write("# SM rwgt_1\n")
    f.write("launch\n")
    if not args.default_operators:
       for op in args.op:
           f.write("   set SMEFT {} {}\n".format(full_ops[op], 0))
    
    else:
       for op in dop:
          f.write("   set SMEFT {} {}\n".format(full_ops[op], 0))

    f.write("\n\n")
    
    
    # for Lin and Quad
    i = 2
    for op in args.op:
        for k in [-1,1]:
            f.write("# {}={} rwgt_{}\n".format(op, k, i))
            if args.change_model:
               f.write("change rwgt_dir rwgt_{}\n".format(op if k==1 else op+"_m1"))
               f.write("change model SMEFTsim_U35_MwScheme_UFO-{}_massless\n".format(op if k==1 else op+"_m1"))
               f.write("launch\n")
               #f.write("models/SMEFTsim_U35_MwScheme_UFO/restrict_{}_massless.dat\n".format(op if k==1 else op+"_m1"))
            else:
               f.write("launch\n")
               for op2 in args.op:
                   
                   if op2 != op:
                       f.write("   set SMEFT {} {}\n".format(full_ops[op2], 0))
                   else:
                       f.write("   set SMEFT {} {}\n".format(full_ops[op], k))  
            i+=1    
            f.write("\n\n")
                             
    for ops in list(combinations(args.op,2)):
        f.write("# {}={}, {}={} rwgt_{}\n".format(ops[0], 1, ops[1], 1, i))
        if args.change_model:
           f.write("change rwgt_dir rwgt_{}_{}\n".format(ops[0], ops[1])) 
           if os.path.isfile("SMEFTsim_U35_MwScheme_UFO/restrict_{}_{}_massless.dat".format(ops[0], ops[1])):
              f.write("change model SMEFTsim_U35_MwScheme_UFO-{}_{}_massless\n".format(ops[0], ops[1]))
              f.write("launch\n")
              #f.write("models/SMEFTsim_U35_MwScheme_UFO/restrict_{}_{}_massless.dat\n".format(ops[0], ops[1]))
           elif os.path.isfile("SMEFTsim_U35_MwScheme_UFO/restrict_{}_{}_massless.dat".format(ops[1], ops[0])):
              f.write("change model SMEFTsim_U35_MwScheme_UFO-{}_{}_massless\n".format(ops[1], ops[0]))
              f.write("launch\n")
              #f.write("models/SMEFTsim_U35_MwScheme_UFO/restrict_{}_{}_massless.dat\n".format(ops[1], ops[0]))
           else: sys.exit("[ERROR] No restriction card in SMEFTsim_U35_MwScheme_UFO for op pair {}".format(ops))

        else:

           f.write("launch\n")
           for op3 in args.op:
               if not op3 in ops:
                    f.write("   set SMEFT {} {}\n".format(full_ops[op3], 0))
               else:
                    f.write("   set SMEFT {} {}\n".format(full_ops[op3], 1)) 
                
        i+=1
                
        f.write("\n\n") 

    print("---> Done")
