#!/usr/bin/python
   
smeft = [
      ['1', 'cG'], 
      ['2', 'cW'], 
      ['3', 'cH'], 
      ['4', 'cHbox'],
      ['5', 'cHDD'],
      ['6', 'cHG'],
      ['7', 'cHW'],
      ['8', 'cHB'],
      ['9', 'cHWB'],
      ['10', 'cuHRe'],
      ['11', 'ctHRe'],
      ['12', 'cdHRe'],
      ['13', 'cbHRe'],
      ['14', 'cuGRe'],
      ['15', 'ctGRe'],
      ['16', 'cuWRe'],
      ['17', 'ctWRe'],
      ['18', 'cuBRe'],
      ['19', 'ctBRe'],
      ['20', 'cdGRe'],
      ['21', 'cbGRe'],
      ['22', 'cdWRe'],
      ['23', 'cbWRe'],
      ['24', 'cdBRe'],
      ['25', 'cbBRe'],
      ['26', 'cHj1'],
      ['27', 'cHQ1'],
      ['28', 'cHj3'],
      ['29', 'cHQ3'],
      ['30', 'cHu'],
      ['31', 'cHt'],
      ['32', 'cHd'],
      ['33', 'cHbq'],
      ['34', 'cHudRe'],
      ['35', 'cHtbRe'],
      ['36', 'cjj11'],
      ['37', 'cjj18'],
      ['38', 'cjj31'],
      ['39', 'cjj38'],
      ['40', 'cQj11'],
      ['41', 'cQj18'],
      ['42', 'cQj31'],
      ['43', 'cQj38'],
      ['44', 'cQQ1'],
      ['45', 'cQQ8'],
      ['46', 'cuu1'],
      ['47', 'cuu8'],
      ['48', 'ctt'],
      ['49', 'ctu1'],
      ['50', 'ctu8'],
      ['51', 'cdd1'],
      ['52', 'cdd8'],
      ['53', 'cbb'],
      ['54', 'cbd1'],
      ['55', 'cbd8'],
      ['56', 'cud1'],
      ['57', 'ctb1'],
      ['58', 'ctd1'],
      ['59', 'cbu1'],
      ['60', 'cud8'],
      ['61', 'ctb8'],
      ['62', 'ctd8'],
      ['63', 'cbu8'],
      ['64', 'cutbd1Re'],
      ['65', 'cutbd8Re'],
      ['66', 'cju1'],
      ['67', 'cQu1'],
      ['68', 'cju8'],
      ['69', 'cQu8'],
      ['70', 'ctj1'],
      ['71', 'ctj8'],
      ['72', 'cQt1'],
      ['73', 'cQt8'],
      ['74', 'cjd1'],
      ['75', 'cjd8'],
      ['76', 'cQd1'],
      ['77', 'cQd8'],
      ['78', 'cbj1'],
      ['79', 'cbj8'],
      ['80', 'cQb1'],
      ['81', 'cQb8'],
      ['82', 'cjQtu1Re'],
      ['83', 'cjQtu8Re'],
      ['84', 'cjQbd1Re'],
      ['85', 'cjQbd8Re'],
      ['86', 'cjujd1Re'],
      ['87', 'cjujd8Re'],
      ['88', 'cjujd11Re'], 
      ['89', 'cjujd81Re'], 
      ['90', 'cQtjd1Re'],
      ['91', 'cQtjd8Re'],
      ['92', 'cjuQb1Re'],
      ['93', 'cjuQb8Re'],
      ['94', 'cQujb1Re'],
      ['95', 'cQujb8Re'],
      ['96', 'cjtQd1Re'],
      ['97', 'cjtQd8Re'],
      ['98', 'cQtQb1Re'],
      ['99', 'cQtQb8Re'],
      ['100', 'ceHRe'],
      ['101', 'ceWRe'], 
      ['102', 'ceBRe'],
      ['103', 'cHl1'],
      ['104', 'cHl3'],
      ['105', 'cHe'],
      ['106', 'cll'], 
      ['107', 'cll1'], 
      ['108', 'clj1'], 
      ['109', 'clj3'], 
      ['110', 'cQl1'], 
      ['111', 'cQl3'], 
      ['112', 'cee'], 
      ['113', 'ceu'], 
      ['114', 'cte'], 
      ['115', 'ced'], 
      ['116', 'cbe'], 
      ['117', 'cje'], 
      ['118', 'cQe'], 
      ['119', 'clu'], 
      ['120', 'ctl'], 
      ['121', 'cld'], 
      ['122', 'cbl'], 
      ['123', 'cle'], 
      ['124', 'cledjRe'],
      ['125', 'clebQRe'], 
      ['126', 'cleju1Re'], 
      ['127', 'cleQt1Re'], 
      ['128', 'cleju3Re'], 
      ['129', 'cleQt3Re'], 
]    

smeftcpv = [
  ['1', 'cGtil'],
  ['2', 'cWtil'], 
  ['3', 'cHGtil'], 
  ['4', 'cHWtil'], 
  ['5', 'cHBtil'], 
  ['6', 'cHWBtil'],
  ['7', 'cuGIm'],
  ['8', 'ctGIm'], 
  ['9', 'cuWIm'], 
  ['10', 'ctWIm'], 
  ['11', 'cuBIm'], 
  ['12', 'ctBIm'], 
  ['13', 'cdGIm'], 
  ['14', 'cbGIm'], 
  ['15', 'cdWIm'], 
  ['16', 'cbWIm'], 
  ['17', 'cdBIm'], 
  ['18', 'cbBIm'], 
  ['19', 'cuHIm'], 
  ['20', 'ctHIm'], 
  ['21', 'cdHIm'], 
  ['22', 'cbHIm'], 
  ['23', 'cHudIm'],
  ['24', 'cHtbIm'],
  ['25', 'cutbd1Im'], 
  ['26', 'cutbd8Im'], 
  ['27', 'cjQtu1Im'], 
  ['28', 'cjQtu8Im'], 
  ['29', 'cjQbd1Im'], 
  ['30', 'cjQbd8Im'], 
  ['31', 'cjujd1Im'], 
  ['32', 'cjujd8Im'], 
  ['33', 'cjujd11Im'], 
  ['34', 'cjujd81Im'], 
  ['35', 'cQtjd1Im'], 
  ['36', 'cQtjd8Im'], 
  ['37', 'cjuQb1Im'], 
  ['38', 'cjuQb8Im'], 
  ['39', 'cQujb1Im'], 
  ['40', 'cQujb8Im'], 
  ['41', 'cjtQd1Im'], 
  ['42', 'cjtQd8Im'], 
  ['43', 'cQtQb1Im'], 
  ['44', 'cQtQb8Im'], 
  ['45', 'ceHIm'], 
  ['46', 'ceWIm'], 
  ['47', 'ceBIm'], 
  ['48', 'cledjIm'], 
  ['49', 'clebQIm'], 
  ['50', 'cleju1Im'], 
  ['51', 'cleju3Im'], 
  ['52', 'cleQt1Im'], 
  ['53', 'cleQt3Im'], 
]   

if __name__ == "__main__":


  f_before = open ('restrict_before_v3_0_topU3l.txt', 'r')
  contents_before = f_before.read ()
  f_before.close ()
  f_after  = open ('restrict_after_v3_0_topU3l.txt', 'r') 
  contents_after = f_after.read ()
  f_after.close ()

  # build restrict cards for single parameters
  # ----------------------------------------------------

  # loop over parameters to be restricted
  for param in smeft:
#    if param[0] == '21' or param[0] == '28': continue ; # already existing in the folder 
    f_restrict = open ('restrict_'+ param[1] + 'm1_massless.dat', 'w')
    f_restrict.write (contents_before)
    # loop over parameters to be written
    for param2 in smeft:
      if param2[0] == param[0]: 
        f_restrict.write ('   ' + param2[0] + ' -9.999999e-01 # ' + param2[1] + '\n')
      else:
        f_restrict.write ('   ' + param2[0] + ' 0.000000 # ' + param2[1] + '\n')
    # loop over parameters to be written
    f_restrict.write("""###################################\n
## INFORMATION FOR SMEFTCPV\n
###################################\n
Block SMEFTcpv\n""" )

    for param2 in smeftcpv:
        f_restrict.write ('   ' + param2[0] + ' 0.000000 # ' + param2[1] + '\n')

    f_restrict.write (contents_after)
    f_restrict.close ()
  # loop over parameters to be restricted
  
  # loop over parameters to be restricted
  for param in smeftcpv:
    f_restrict = open ('restrict_'+ param[1] + 'm1_massless.dat', 'w')
    f_restrict.write (contents_before)

    # loop over parameters to be written
    for param2 in smeft:
      f_restrict.write ('   ' + param2[0] + ' 0.000000 # ' + param2[1] + '\n')
    # loop over parameters to be written
    f_restrict.write("""###################################\n
## INFORMATION FOR SMEFTCPV\n
###################################\n
Block SMEFTcpv\n""" )

    for param2 in smeftcpv:
      if param2[0] == param[0]:
        f_restrict.write ('   ' + param2[0] + ' -9.999999e-01 # ' + param2[1] + '\n')
      else:
        f_restrict.write ('   ' + param2[0] + ' 0.000000 # ' + param2[1] + '\n')

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
          f_restrict.write ('   ' + param2[0] + ' 0.000000            # ' + param2[1] + '\n')

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
          f_restrict.write ('   ' + param2[0] + ' 0.000000            # ' + param2[1] + '\n')
      # loop over parameters to be written

      f_restrict.write (contents_after)
      f_restrict.close ()




           
