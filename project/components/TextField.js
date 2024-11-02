import React from "react";

const TextField = (props) => {
    const {type, placeholder} = props
    return (
        <input 
            type={type}
            className="border focus:border-d-blue border-gray-500 rounded-xl placeholder-gray-400 bg-white text-lg p-[8px] h-auto mb-[10px] w-[60%]"
            placeholder={placeholder}
        />
    );
};

export default TextField;
