================================================================================
----------------------------  Guide for this analysis   ------------------------
================================================================================

All code is stored in a repository named :
Analyse$$$  with $$$  the batch number

fig is a repository with old figures

legacy code is a compilation of quick macro that can be executed with a .x on 
the root terminal to get a quick idea of the data.





--------------------------------------------------------------------------------

----------------------------    Getting started     ----------------------------

--------------------------------------------------------------------------------


In every repository there is 3 sub-repo : fig, Analyse_Altiroc and Analyse_centre
   fig is where the figures are stored
   Analyse_Altiroc is an analysis with a cut at 2fC
   Analyse_Centre is an analysis for the central part of the sensors

Some of the Analyse repository have more sub-repo.
In this case this is a test sub-repo that is yet to be implemented.



There is also several files wich are respectively :
   1.monanalyse_*.c  
   Which is the source code in C++ for the analysis

   2.monanalyse_*.h
   Which is the header file for the source code
   
   3.tree_date_batch.root
   Which is the NTuple for the analysis
   
   4.All .txt file
   Results of the fit on each bin of the DUT
   
   5.There might be some macro who come from legacy code
   
   6. GLandauFit.C
   A c++ source code which return the MPV and MPVerr in bin of the DUT
   for a fit of the charge by a landau convoluted by a gaussian
   
Each analysis can be compiled on root with the following syntax :
   .L monanalyse_*.C
   monanalyse m
   m.Loop()
   
It will produce several plot stored in the fig sub-repo.


--------------------------------------------------------------------------------

----------------------------    Getting started     ----------------------------

--------------------------------------------------------------------------------





--------------------------------------------------------------------------------

--------------------------- .C Changing BATCH      -----------------------------

--------------------------------------------------------------------------------


If you need to analyse a batch that is not in the code which is very likely, 
follow this procedure to adapt the code to your need.

   1. Copy the .C and monanalyse.h file to the same directory where your data 
   .root file is located.
   
   2.  Modify the line 163 and 165 of the monanalyse.h file to fit the path of
   your data.
   
      if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("YOUR_TREE_PATH_HERE.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("YOUR_TREE_PATH_HERE.root");
      }
      f->GetObject("tree",tree);
   
   3. Modify the .C file.
   You will want to modify the ShiftX and ShiftY variables
   under the Definitions part, put the coordinate of the center of your DUTs
   in mimosa unit here.
   To find the center of the DUT I advice to do a simple:
   
   TFile *input = new TFile("YOUR_TREE_PATH_HERE.root");
   TTree *tree = (TTree*)input->Get("tree"); 
   tree->Draw("Xtr[NDUT]:Ytr[NDUT]>>h1","Colz");
   
   Also modify the cuts for noise removal, I advice to check all those cuts
   individually for each batch as some may need to be changed from batch to batch.
   
   4. Run the code and solve any error that may arise.

--------------------------------------------------------------------------------

--------------------------- .C Changing BATCH      -----------------------------

--------------------------------------------------------------------------------





--------------------------------------------------------------------------------

--------------------------- .C file survival kit -------------------------------

--------------------------------------------------------------------------------



All the c++ source code have the same structure, here is a survival kit on how
to navigate through the code.

The structure of the codes are divided in several part. All part start and end with 
a line of comment of the following format : 

//==============================================================================
//-------------------------------name of the part-------------------------------
//==============================================================================

Here is the standard part of a .C code :

0 . Header
   Here you will find all the header to make the code work out fine. 
   Some header are not necessary so if there is a need to optimise the code feel
   free to delete some of it.
   
   
   
1 . Definitions
   Here you will find all standard définitions that are outside of the loop.
   This include variables to change the scale of geometric plot from mimosa unit
   to mm. 
   
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   IF YOU WANT TO ANALYSE ANOTHER BATCH YOU NEED TO MODIFY THE shiftX and
   shiftY VARIABLES, this is explained in further details in the previous part
   of this txt.
   
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   This part of the code is here to center the DUTS on 2D histogram.
   
   
   
2. Initialisations
   Here you will find all the variables declaration and initialisation for the 
   code. 
   If you were to add something to the analysis this is where you need to create
   your variable. Make sure every variable is initialised.
   
   The most importants parts of this section are the Standards variables part 
   and the CUT part. Let's see why :
   
                                 Standards variables
    
   Here is defined all the important variables of the code.
   
   NDUT dictate the number of dut and therefore the size of each structure in 
   the code, manipulate this variable with caution.
   
   Nbin give the standard binning for 1D histogram.
   
   NbinBox give the binning for 2D histograms, only the first element is used in
   the code.
   
   NbinCenter define the center of the DUT for 2D histogram.
   
   BinSizePos is the size of a bin in mm. it's not initialised here but later on.
   
   NEventMin give the minimum number of event for a bin of a 2D histogram to not
   be considered as noise.
   
   
   
                                       CUT
   Here is defined the cut on data to remove noise. 
   
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   THOSE CUTS ARE SPECIFIC TO THE BATCH STUDIED AND MUST BE CHECK EACH TIME AN
   OTHER BATCH IS STUDIED. EXPECT FOR BATCH 205 AND 201 THOSE CUTS WERE NOT
   ADAPTED TO FIT EACH BATCH.
   
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   If you want your analysis to work you will need to modify those cut.
   Specifically TimeMin and TimeMax for the cut on difference between 
   TimeAtMax[SiPM] and TimeAtMax[DUT].
   
   ChargeCut is the minimum charge applied for each event. If you modify this 
   var in GLandauFit.C make sure to change the range of the RooRealVar variable,
   if you dont the fitting will be weird.
   
   
   
   
3. LOOP
   Here is the core of the loop.
   This is where you fill your histograms with the tree data.
   
   
4. Analysis
   Here you will find all manipulation of previously filled histograms that
   does not plot them. For instance this is where the fitting take place.
   
5. Plot
   You can make some plot here.
   
   
--------------------------------------------------------------------------------

--------------------------- .C file survival kit -------------------------------

--------------------------------------------------------------------------------







































