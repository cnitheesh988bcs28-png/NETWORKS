static int cmp_int(const void* a, const void* b) {
    int x = *(const int*)a, y = *(const int*)b;
    return (x > y) - (x < y);
}

static int lower_bound_int(const int* a, int n, int t) {
    int left = 0, right = n;
    while (left < right) {
        int mid = (left + right) >> 1;
        if (a[mid] < t)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

static void bit_add(int* bit, int size, int index, int delta) {
    for (; index <= size; index += index & -index)
        bit[index] += delta;
}

static int bit_find_kth(int* bit, int size, int k) {
    int index = 0;
    int bit_mask = 1 << 30;
    while (bit_mask > 0) {
        int next_idx = index + bit_mask;
        if (next_idx <= size && bit[next_idx] < k) {
            k -= bit[next_idx];
            index = next_idx;
        }
        bit_mask >>= 1;
    }
    return index + 1; // 1-based index
}

double* medianSlidingWindow(int* nums, int numsSize, int k, int* returnSize) {
    *returnSize = numsSize - k + 1;
    double* result = (double*)malloc(sizeof(double) * (*returnSize));
    
    int* sorted_vals = (int*)malloc(sizeof(int) * numsSize);
    for (int i = 0; i < numsSize; ++i)
        sorted_vals[i] = nums[i];
    qsort(sorted_vals, numsSize, sizeof(int), cmp_int);
    
    int uniqueCount = 0;
    for (int i = 0; i < numsSize; ++i) { // Ignore dupes
        if (i == 0 || sorted_vals[i] != sorted_vals[i - 1])
            sorted_vals[uniqueCount++] = sorted_vals[i];
    }
    
    // Initialize Fenwick Tree
    int* bit = (int*)calloc(uniqueCount + 1, sizeof(int));
    
    for (int i = 0; i < k; ++i) {
        int id = lower_bound_int(sorted_vals, uniqueCount, nums[i]) + 1;
        bit_add(bit, uniqueCount, id, 1);
    }
    double getMedian(int total) {
        if (k % 2 == 1) {
            int median_idx = bit_find_kth(bit, uniqueCount, (total + 1) / 2);
            return (double)sorted_vals[median_idx - 1];
        } else {
            int median_idx1 = bit_find_kth(bit, uniqueCount, total / 2);
            int median_idx2 = bit_find_kth(bit, uniqueCount, total / 2 + 1);
            return ((double)sorted_vals[median_idx1 - 1] + (double)sorted_vals[median_idx2 - 1]) / 2.0;
        }
    }

    int idx = 0;
    result[idx++] = getMedian(k);
    
    for (int i = k; i < numsSize; ++i) {
        int rem_id = lower_bound_int(sorted_vals, uniqueCount, nums[i - k]) + 1;
        int add_id = lower_bound_int(sorted_vals, uniqueCount, nums[i]) + 1;
        bit_add(bit, uniqueCount, rem_id, -1);
        bit_add(bit, uniqueCount, add_id, 1);
        result[idx++] = getMedian(k);
    }
    return result;
}
