#!/usr/bin/python
   
smeft = [
   [ '1' , 'cG' ],
   [ '2' , 'cW' ],
   [ '3' , 'cH' ],
   [ '4' , 'cHbox' ],
   [ '5' , 'cHDD' ],
   [ '6' , 'cHG' ],
   [ '7' , 'cHW' ],
   [ '8' , 'cHB' ],
   [ '9' , 'cHWB' ],
   [ '10' , 'ceHRe' ],
   [ '11' , 'cuHRe' ],
   [ '12' , 'cdHRe' ],
   [ '13' , 'ceWRe' ],
   [ '14' , 'ceBRe' ],
   [ '15' , 'cuGRe' ],
   [ '16' , 'cuWRe' ],
   [ '17' , 'cuBRe' ],
   [ '18' , 'cdGRe' ],
   [ '19' , 'cdWRe' ],
   [ '20' , 'cdBRe' ],
   [ '21' , 'cHl1' ],
   [ '22' , 'cHl3' ],
   [ '23' , 'cHe' ],
   [ '24' , 'cHq1' ],
   [ '25' , 'cHq3' ],
   [ '26' , 'cHu' ],
   [ '27' , 'cHd' ],
   [ '28' , 'cHudRe' ],
   [ '29' , 'cll' ],
   [ '30' , 'cll1' ],
   [ '31' , 'cqq1' ],
   [ '32' , 'cqq11' ],
   [ '33' , 'cqq3' ],
   [ '34' , 'cqq31' ],
   [ '35' , 'clq1' ],
   [ '36' , 'clq3' ],
   [ '37' , 'cee' ],
   [ '38' , 'cuu' ],
   [ '39' , 'cuu1' ],
   [ '40' , 'cdd' ],
   [ '41' , 'cdd1' ],
   [ '42' , 'ceu' ],
   [ '43' , 'ced' ],
   [ '44' , 'cud1' ],
   [ '45' , 'cud8' ],
   [ '46' , 'cle' ],
   [ '47' , 'clu' ],
   [ '48' , 'cld' ],
   [ '49' , 'cqe' ],
   [ '50' , 'cqu1' ],
   [ '51' , 'cqu8' ],
   [ '52' , 'cqd1' ],
   [ '53' , 'cqd8' ],
   [ '54' , 'cledqRe' ],
   [ '55' , 'cquqd1Re' ],
   [ '56' , 'cquqd11Re' ],
   [ '57' , 'cquqd8Re' ],
   [ '58' , 'cquqd81Re' ],
   [ '59' , 'clequ1Re' ],
   [ '60' , 'clequ3Re' ]
]    

smeftcpv = [
   [ '1' , 'cGtil' ],
   [ '2' , 'cWtil' ],
   [ '3' , 'cHGtil' ],
   [ '4' , 'cHWtil' ],
   [ '5' , 'cHBtil' ],
   [ '6' , 'cHWBtil' ],
   [ '7' , 'ceWIm' ],
   [ '8' , 'ceBIm' ],
   [ '9' , 'cuGIm' ],
   [ '10' , 'cuWIm' ],
   [ '11' , 'cuBIm' ],
   [ '12' , 'cdGIm' ],
   [ '13' , 'cdWIm' ],
   [ '14' , 'cdBIm' ],
   [ '15' , 'cHudIm' ],
   [ '16' , 'ceHIm' ],
   [ '17' , 'cuHIm' ],
   [ '18' , 'cdHIm' ],
   [ '19' , 'cledqIm' ],
   [ '20' , 'cquqd1Im' ],
   [ '21' , 'cquqd8Im' ],
   [ '22' , 'cquqd11Im' ],
   [ '23' , 'cquqd81Im' ],
   [ '24' , 'clequ1Im' ],
   [ '25' , 'clequ3Im' ]
]    

if __name__ == "__main__":


  f_before = open ('restrict_before_v3_0.txt', 'r')
  contents_before = f_before.read ()
  f_before.close ()
  f_after  = open ('restrict_after_v3_0.txt', 'r') 
  contents_after = f_after.read ()
  f_after.close ()

  # build restrict cards for single parameters
  # ----------------------------------------------------

  # loop over parameters to be restricted
  for param in smeft:
#    if param[0] == '21' or param[0] == '28': continue ; # already existing in the folder 
    f_restrict = open ('restrict_'+ param[1] + '_massless.dat', 'w')
    f_restrict.write (contents_before)
    
    # loop over parameters to be written
    for param2 in smeft:
      if param2[0] == param[0]: 
        f_restrict.write ('   ' + param2[0] + ' 9.999999e-01 # ' + param2[1] + '\n')
      else:
        f_restrict.write ('   ' + param2[0] + ' 0 # ' + param2[1] + '\n')
    # loop over parameters to be written
    f_restrict.write("""###################################\n
## INFORMATION FOR SMEFTCPV\n
###################################\n
Block SMEFTcpv\n""" )

    for param2 in smeftcpv:
        f_restrict.write ('   ' + param2[0] + ' 0 # ' + param2[1] + '\n')

    f_restrict.write (contents_after)
    f_restrict.close ()
  # loop over parameters to be restricted
  
  # loop over parameters to be restricted
  for param in smeftcpv:
    f_restrict = open ('restrict_'+ param[1] + '_massless.dat', 'w')
    f_restrict.write (contents_before)

    # loop over parameters to be written
    for param2 in smeft:
      f_restrict.write ('   ' + param2[0] + ' 0 # ' + param2[1] + '\n')
    # loop over parameters to be written
    f_restrict.write("""###################################\n
## INFORMATION FOR SMEFTCPV\n
###################################\n
Block SMEFTcpv\n""" )

    for param2 in smeftcpv:
      if param2[0] == param[0]:
        f_restrict.write ('   ' + param2[0] + ' 9.999999e-01 # ' + param2[1] + '\n')
      else:
        f_restrict.write ('   ' + param2[0] + ' 0 # ' + param2[1] + '\n')

    f_restrict.write (contents_after)
    f_restrict.close ()

  # build restrict cards for double parameters
  # ----------------------------------------------------

  # sort the list alphabetically according to the parameter name
  from operator import itemgetter
  sortedpar = sorted (smeft + smeftcpv, key = itemgetter (1))

  for i in range (len (smeft + smeftcpv)):
    for j in range (i+1, len (smeft + smeftcpv)):
      f_restrict = open ('restrict_' + sortedpar[i][1] + '_' + sortedpar[j][1] + '_massless.dat', 'w')
      f_restrict.write (contents_before)
      
      # loop over parameters to be written
      for param2 in smeft:
        if param2[1] == sortedpar[i][1]: 
          f_restrict.write ('   ' + param2[0] + ' 9.999999e-01 # ' + param2[1] + '\n')
        elif param2[1] == sortedpar[j][1]:
          f_restrict.write ('   ' + param2[0] + ' 9.999999e-01 # ' + param2[1] + '\n')
        else: 
          f_restrict.write ('   ' + param2[0] + ' 0            # ' + param2[1] + '\n')

      f_restrict.write("""###################################\n
## INFORMATION FOR SMEFTCPV\n
###################################\n
Block SMEFTcpv\n""" )

      for param2 in smeftcpv:
        if param2[1] == sortedpar[i][1]:
          f_restrict.write ('   ' + param2[0] + ' 9.999999e-01 # ' + param2[1] + '\n')
        elif param2[1] == sortedpar[j][1]:
          f_restrict.write ('   ' + param2[0] + ' 9.999999e-01 # ' + param2[1] + '\n')
        else:
          f_restrict.write ('   ' + param2[0] + ' 0            # ' + param2[1] + '\n')
      # loop over parameters to be written

      f_restrict.write (contents_after)
      f_restrict.close ()




           
