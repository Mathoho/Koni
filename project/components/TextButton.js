import React from "react";

const TextButton = (props) => {
    const {text} = props
    return (
    <button className="w-[60%] font-lilita text-2xl rounded-xl bg-gradient-to-r from from-d-purple to to-d-blue text-white py-[6px]">{text}</button>
    );
};

export default TextButton;