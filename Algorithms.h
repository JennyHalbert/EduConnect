#pragma once
#include <vector>
#include <functional> // For std::function


template <typename T, typename Comparator>
void merge(std::vector<T>& arr, int left, int mid, int right, Comparator comp) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary vectors
    std::vector<T> L(n1);
    std::vector<T> R(n2);

    // Copy data to temp vectors
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        // Use the custom comparator 'comp'
        // If comp(a, b) is true, 'a' goes first.
        if (comp(L[i], R[j])) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

template <typename T, typename Comparator>
void merge_sort(std::vector<T>& arr, int left, int right, Comparator comp) {
    if (left < right) {
        // Same as (left+right)/2, but avoids overflow for large left and right
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        merge_sort(arr, left, mid, comp);
        merge_sort(arr, mid + 1, right, comp);

        // Merge the sorted halves
        merge(arr, left, mid, right, comp);
    }
}