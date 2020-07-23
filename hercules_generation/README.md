La generazione si basa sul [framework di CMS-SW genproductions](https://github.com/cms-sw/genproductions/) e sulle [cards scritte da Jie](https://github.com/freejiebao/generator/tree/master/MG5CARDS/SSWW_RcW_int)


Il metodo con cui generavo eventi su Hercules era il seguente:

1. Creavo la param_card e la proc_cards manualmente selezionando ogni volta l'operatore
2. Copiavo queste cards insieme alla SSWW_RcW_bsm_extramodels.dat e alla run_card.dat che usava Jie in una cartella che veniva usata da submit_condor_gridpack_generation.sh per creare la gridpack
3. lanciavo la generazione della gridpack e creavo il file .sub per condor
4. Facevo il submit di 100 jobs con il file .sub



Gli ultimi tre passaggi sono svolti in automatico dallo script2.sh a cui passavo semplicemente il nome della cartella che di solito era operatore_tipo-di-distribuzione (es. Hl1_lin)
