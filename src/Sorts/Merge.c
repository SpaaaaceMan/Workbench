/**
 * Merge two sorted tables (which are in fact inside tab).
 * @param tab The table containing all values.
 * @param beginTab1 The begginnin index of first tab.
 * @param endTab1 The ending index of the first tab.
 * @param endTab2 The ending index of the second tab.
 * @author Thomas MEDARD
 */
void mergeTab(int tab[], const size_t beginTab1, const size_t endTab1, const size_t endTab2) {
	// Array used for merging
    int temp[(endTab2 - beginTab1) + 1];    
    size_t i, j, k;
	
	// Beginning of the first list
    i = beginTab1;  
	// Beginning of the second list	
    j = endTab1 + 1;    
	// Tmp iterator
    k = 0;
    
	// While elements in both lists
    while(i <= endTab1 && j <= endTab2)	{
        if(tab[i] < tab[j]) {
            temp[k++] = tab[i++];
		}
        else {
            temp[k++] = tab[j++];
		}
    }
    
	// Copy remaining elements of the first list
    while(i <= endTab1) {    
        temp[k++] = tab[i++];
	}
        
	// Copy remaining elements of the second list
    while(j <= endTab2) {  
        temp[k++] = tab[j++];
	}
	
    // Transfer elements from temp[] back to tab[]
    for(i = beginTab1, j = 0; i <= endTab2; i++, j++) {
        tab[i] = temp[j];
	}
}

/**
 * Main recursion : cut tab in half (with indexes) and then try to merge them when the are sorted.
 * @param begin The begginning index.
 * @param end The ending index.
 * @author Thomas MEDARD
 */
void mergesort(int tab[], size_t begin, size_t end) {        
    if(begin < end) {
		// Middle
        size_t mid = (begin + end) / 2;		 
		
		// Left recursion
        mergesort(tab, begin, mid);  
		// Right recursion        
        mergesort(tab, mid + 1 , end);       
		
		// Merging of two sorted tables
        mergeTab(tab, begin , mid, end); 
    }
}

/*
 * Sort an int table with merge algorithim.
 * @param tab The table of int to sort.
 * @param tabSize the size of tab.
 * @author Thomas MEDARD
 */
void merge (int tab[], size_t tabSize) {
	
	mergesort(tab, 0, tabSize - 1);
}