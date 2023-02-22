# Incremental Joinable Tables Search

//  This is the README file for the dstree C implementation.
//  The dstree is a Data-adaptive and Dynamic Segmentation 
//  Index for Whole Matching on Time Series.
// 
//  URL link for the DSTREE paper:
//       https://www.cs.sfu.ca/~jpei/publications/time%20series%20index%20VLDB13.pdf
//  The original implementation was in Java.
//
//  This C version was developed by:
//  Karima Echihabi on 18/12/2016 and
//  Jaouhara Chanchaf on 02/22/2023

//  Copyright 2016 Paris Descartes University. All rights reserved.
//  Copyright 2023 Mohammed 6 Polytechnic University. All rights reserved.

//  This code is based on the isax code written by Zoumpatianos on 3/12/12.
//  Copyright 2012 University of Trento. All rights reserved.

--To compile and run, go to code directory: 

-------------------DO ONCE TO SET UP ENVIRONMENT--------------------
--Run 'aclocal' to generate an m4 environment for autotools to use:
aclocal

--Then run autoconf to turn our configure.ac into a configure script:
autoconf

--Then run automake to turn our Makefile.am into a Makefile.in:
automake --add-missing


-----------------------------COMPILE-------------------------------
./configure
make

------------------------------RUN----------------------------------
bin/dstree --help


------------------------------EXAMPLES----------------------------------

--------------------------------------------------------------------
                     BUILDING THE INDEX
--------------------------------------------------------------------
bin/dstree --dataset /data/real/jchanchaf/wdc-2015-en-full/subsets/100-nonorm/ 
--total-data-files 100 --buffer-size 600 --index-path /home/jaouhara.chanchaf/work-dir/indexes/100-nonorm-idx/ 
--ascii-input 0 --mode 0 --track-bsf   --incremental --delta 1 --epsilon 0 --k 100 
--timeseries-size 50 --track-vector 1 --warping 0.00 --leaf-size 100


--------------------------------------------------------------------
                     QUERYING THE INDEX
--------------------------------------------------------------------
bin/dstree --queries /data/real/jchanchaf/wdc-2015-en-full/10m/query/10q-size100/ 
 --total-data-files 100000 --buffer-size 600 --index-path /home/jaouhara.chanchaf/work-dir/indexes/100k-nonorm-idx/
  --ascii-input 0 --mode 1 --track-bsf   --incremental --delta 1 --epsilon 0  
  --timeseries-size 50 --track-vector 1 --warping 0.00 --leaf-size 100000 
  --top 10 --dataset /data/real/jchanchaf/wdc-2015-en-full/subsets/100-nonorm/ 
  --result-dir /home/jaouhara.chanchaf/work-dir/exp-results/kashif-search/100tk/stop-mode-0/ 
  --ground-truth-dir ./na/  --nq 10 --min-qset-size 100 --max-qset-size 100   
  --parallel --incremental --store-results-in-disk 1 --stop-when-nn-dist-changes 0 --k  10  
  --k-values "5,10" --knn-data-structure minmax-heap
The parameter buffer-size is in MBs so in this example, the buffer is ~ 0.6 GB.
--------------------------------------------------------------------
                     PARAMETERS
--------------------------------------------------------------------
--top [x] : nb of matching columns to be retrieved.
--timeseries-size [x] : length of the query vectors.
--nq [x] : number of query column to execute.
--max-qset-size : Maximum query column size.
--min-qset-size : Minumum query column size.
--num-threads [x] : number of worker threads. 
--k : (kmax) number of nearest neigbors to retrieve for each query vector in the query column.
--k-values [string] : intermediate k values for which we would like to measure kashif performance (query time, recall) (only works for incremental search).

--knn-data-structure [string] : data structure used to store nearest neighbors. Options: sorted-arr, minmax-heap, ostree.
--stop-when-nn-dist-changes [0/1] : only return nns of the same distance as the first nn.
--result-dir : directory where query results will be stored
--ground-truth-dir : directory where ground truth results are stored (necessary when measuring recall)
--store-results-in-disk [1/0] : set it to 0 is you don't want query results to be stored in disk





