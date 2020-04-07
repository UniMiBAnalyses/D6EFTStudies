# sparse notes on the plotting of the results

## if I wanted to use matplotlib instead of root

  * TTree to DataFrame: 
  * matplotlib pyplot contours: [matplotlib page](https://matplotlib.org/3.1.1/api/_as_gen/matplotlib.pyplot.contour.html)
  * get matplotlib contours: [stackOverflow](https://stackoverflow.com/questions/5666056/matplotlib-extracting-data-from-contour-lines)

## suggerimenti su come usare combine

  * [macro](https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit/blob/74x-root6/test/plotting/contours2D.cxx)
  * [macro](https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit/blob/81x-root606/test/multiDim/contourPlot.cxx)
  * snippet to calculate areas
  ```
    points=[]
    for x in range(0,th2.GetNbinsX()): 
       for y in range(0,th2.GetNbinsX()):
          points.append( th2.GetXaxis().GetBinCenter(x+1), ...,  )
    area=len([ (x,y,z) for x,y,z in points if z< 6.18]) * th2.GetXaxis().GetBinWidth(1)*th2.GetYaxis().GetBinWidth(1) ## assuming constant binning
  ```
  * snippet to draw contours
  ```
  cont_1sig = th2.Clone('cont_1_sig')
  cont_1sig.SetContour(2)
  cont_1sig.SetContourLevel(1,2.3)
  cont_1sig.SetLineColor((options.colors[fi]))
  cont_1sig.SetLineWidth(3)
  cont_1sig.SetLineStyle(1)
  cont_2sig = th2.Clone('cont_2_sig')
  cont_2sig.SetContour(2)
  cont_2sig.SetContourLevel(1,6.18)
  cont_2sig.SetLineColor((options.colors[fi]))
  cont_2sig.SetLineWidth(3)
  cont_2sig.SetLineStyle(2)
  if options.get2dhist: 
    cont_2sig.SetLineStyle(1)
    cont_2sig.SetLineWidth(2)
  ```    