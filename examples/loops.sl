// While loop examples
// Count from 1 to 5
let i = 1
while i <= 5 do
    print("i =", i)
    let i = i + 1
end

// Sum of first N numbers
let n = 10
let sum = 0
let counter = 1

while counter <= n do
    let sum = sum + counter
    let counter = counter + 1
end

print("Sum of first", n, "numbers is", sum)

// Fibonacci sequence
let limit = 20
let a = 0
let b = 1

print("Fibonacci sequence up to", limit, ":")

while a <= limit do
    print(a)
    let temp = a + b
    let a = b
    let b = temp
end

// Break-like pattern
let found = false
let search = 42
let current = 1

while !found && current <= 100 do
    if current == search then
        print("Found", search, "at position", current)
        let found = true
    end
    let current = current + 1
end

if !found then
    print(search, "not found in range 1-100")
end