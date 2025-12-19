// Conditional statements examples
let age = 18

if age >= 18 then
    print("You are an adult")
else
    print("You are a minor")
end

// Nested if statements
let score = 85

if score >= 90 then
    print("Grade: A")
else if score >= 80 then
    print("Grade: B")
else if score >= 70 then
    print("Grade: C")
else if score >= 60 then
    print("Grade: D")
else
    print("Grade: F")
end

// Logical operators
let hasTicket = true
let hasMoney = false

if hasTicket && hasMoney then
    print("You can enter and buy snacks")
else if hasTicket then
    print("You can enter but cannot buy snacks")
else
    print("You cannot enter")
end

// Complex condition
let temperature = 25
let isSummer = true

if (temperature > 30 || (temperature > 20 && isSummer)) then
    print("It's hot today!")
else
    print("It's cool today")
end