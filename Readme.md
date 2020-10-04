# FM-index 

Release Date: 21th September, 2020

Author

	~Changyong Yu (Northeastern University in CHINA)
	~Chu Zhao (Northeastern University in CHINA)
	~Yu Wang (Northeastern University in CHINA)

1.Introduction
--

  FM-index is a space-time effective suffix search algorithm proposed by Ferragina and Manzini in 2000,which is based on the BWT algorithm.

  Be different of the suffix arrays, FM-index is an index structure that supports backward search. It adds Occ array and BWT string to record the context information of characters on the basis of suffix array. It is a lossless data compression structure.

  The compressed FM-index is composed of three parts of information, 1)BWT,2)SA,3)occ array,c array. BWT is the abbreviation of The Burrows-Wheeler Transform.It is the transformation method of the reference genome,that is,the reference genome is lexicographically reordered.The purpose of the transformation is to facilitate subsequent search.The SA is the suffix array, which saves the true position in the original sequence of the suffix’s first character in the BWT matrix;occ(i,r) represents the number of character i that appear before the rth line in the BWT string;c[i] represents the number of all characters whose lexicographic order is less than character i. 

  The time complexity of FM-index search is O(m), which has nothing to do with the sequence length in the reference library (m is the length of the read).In the FM-index algorithm, the space required for each base is 0.5-2 bytes, which greatly reduces space consumption and can complete an accurate search for a query string in linear time.

  In general, FM-index can effectively construct a lossless index for large-scale genomes.We believe that the tool will improve the performance of data analysis with the bottleneck of large-scale data.


2.Parameters of FM-index
--

	# description of FM-index pararmeters
	
	-h 
	This parameter indicate the reference file position of FM-index input.
	
	-o 
	This parameter indicate the gap of Occ array,i.e.,how many characters are stored once.
	
	-s
	This parameter indicate the gap of SA array,same as above-mentioned parameter
	
	-l
	This parameter used to optimize the memory usage during building process, larger l maybe run longer.
		
	-t
	This parameter is the thread used when building the BWT string, larger t will accelerate the build process.
	
	You can run this program like this: ./FM-index -h example.fa -s 10 -o 10 -l 1 -t 1

3.License
--

	See LICENSE.txt

4.Contacts
--

	Please e-mail your feedback at cyyneu@126.com.



