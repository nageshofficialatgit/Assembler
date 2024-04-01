; Linear search implementation in assembly language

; Define data to search
search_data: data 7       ; Data to search for
              data 3
              data 5
              data 2
              data 9
              data 4
              data 8
              data 6
              data 1

; Define the main program
main:
    ldc search_data      ; Load the memory address of search_data into A
    ldc 9                ; Load the number of elements to search through
    ldc 0                ; Load the initial index as 0
    ldc 0                ; Load the accumulator with 0 to store the found index

search_loop:
    ldl 2                ; Load the current index into A
    ldl 2                ; Load the current index into B (for incrementing)
    ldc 4                ; Load the size of an element (4 bytes)
    mul                   ; Multiply the index by the size of an element
    add                   ; Add the offset to the base address of the data array
    ldnl 0               ; Load the current element to search into A

    ldl 1                ; Load the current search data into B

    sub                   ; Check if the current element matches the search data
    brz found             ; If the element matches, jump to 'found'

    ldc 1                ; Load 1 into A
    ldl 2                ; Load the current index into B
    add                   ; Increment the index
    stl 2                ; Store the updated index

    ldc 9                ; Load the length of the array
    ldl 2                ; Load the current index into A
    sub                   ; Check if the index is at the end of the array
    brz search_done       ; If index == length, search is done

    br search_loop        ; Continue the loop

found:
    ldl 2                ; Load the index of the found element into A
    stl 3                ; Store the found index

search_done:
    HALT                  ; Stop the execution
