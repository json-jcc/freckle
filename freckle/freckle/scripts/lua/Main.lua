
kanas = require('kanas')

Class1 = 
{
    set_point = function(t, v)
        print("this is set_point, " .. v)
    end
}

Class2 =
{
    set_point = function(t, v)
        print("this is set_point 2, " .. v)
    end
}


ClassExample = {

    -- table[key]  
    -- param : table, key
    -- return : one result 
    __index = function(t, k)
        
        local mt = getmetatable(t)

        if mt == nil then
            return
        end
        
        local method = mt.functions[k]
        if method ~= nil then
            return method
        end

        local getter = mt.getters[k]

        if getter ~= nil and type(getter) == "function" then
            return getter(t)
        end

        -- recur supers
        for _, v in ipairs(mt.supers) do
            local member = v[k]

            if member ~= nil then
                return member
            end
        end
        
        return nil
    end,

    -- table[key] = value
    -- params : t, k, v
    __newindex = function(t, k, v)

        local mt = getmetatable(t)
        local setter = mt.setters[k]

        if setter ~= nil and type(setter) == "function" then
            setter(t, v)
        end
        
    end,
    
    supers =
    {
        [1] = Class1,
        [2] = Class2
    },
    
    getters = 
    {
        public_v = function(t)
            print("public_v getter")
            return nil
        end
    },

    setters = 
    {
        public_v = function(t, v)
            print("public_v setter: " .. v)
        end
    },
    
    functions =
    {
        set_values = function(t, v)
            print("set_values : " .. v)
        end
    },
    
}

ClassExampleMeta =
{
    __call = function(self)
        t = {}
        setmetatable(t, ClassExample)
        return t
    end
}

setmetatable(ClassExample, ClassExampleMeta)

test_oda = function()
    
    --local t = {}
    --setmetatable(t, ClassExample)

    local pipeline = ClassExample()

    -- pipeline:set_point(0.8)
    -- pipeline:set_values("sss")
    
    pipeline:set_values("aa")
    print(pipeline.a)
    
    local w = widget.new()
    w:calc(1, 2)
    
    local t = { ["aa"] = 112, ["wa"] = 11232, ["wwx"] = 45}
    for k, v in pairs(t) do 
        print(k .. " : " .. v) 
    end

    w:set_values(t);
    local result = w.calc(2, 10)
    if result == nil then 
        return
    end
    
    kanas.log("w.calc : " .. result)
    kanas.log("w.calc : " .. w.calc(11, 12))
    kanas.log("w.calc : " .. w.calc(23, 2))
    kanas.log("w.calc : " .. w.calc(27, 22))

    print(w)

    kanas.clog("replace : " .. w.replace(w, w))

    local result = w:replace(w)

    if result ~= nil then
       print("replace : " .. result)
    end
    

end

test_oda()

