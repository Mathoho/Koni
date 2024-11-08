import React from "react";

const OptionsButton = () => {
    const handleClick = () => {
        alert("Options Clicked")
    }

    return (
        <button onClick={handleClick} className="px-[15px] py-[5px] rounded-full">
            <div>
                <svg width="6" height="20" viewBox="0 0 6 20" fill="none" xmlns="http://www.w3.org/2000/svg">
                <circle cx="2.59259" cy="2.59259" r="2.59259" fill="black"/>
                <circle cx="2.59259" cy="17.4074" r="2.59259" fill="black"/>
                <circle cx="2.59259" cy="9.99994" r="2.59259" fill="black"/>
                </svg>
            </div>
        </button>
    );
};

export default OptionsButton;