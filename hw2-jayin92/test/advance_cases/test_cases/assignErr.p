/**
 * assignErr.p: LHS of assignment cannot be the result of function call
 */
//&T-
assignErr;

func():integer
begin
        return 321;
end
end

begin
        func() := 123; 
end
end
