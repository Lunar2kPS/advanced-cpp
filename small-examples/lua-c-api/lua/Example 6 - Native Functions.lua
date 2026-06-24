--- Calculates the squared length of the hypotenuse of a triangle given lengths of the shortest 2 sides, a and b.
--- @param a number The length of the first side.
--- @param b number The length of the second side.
--- @return number csqr The squared length of the hypotenuse of the triangle, that is, c².
--- @return number a The original first side length.
--- @return number b The original second side length.
function pythagoras(a, b)
    csqr = nativePythagoras(a, b)
    return csqr, a, b
end
