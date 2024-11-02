import React from "react";

const CategoryChip = (props) => {
    const {title} = props

    return (
        <div className="bg-white rounded-full px-[18px] py-[3px] font-inter text-xs border border-black">
            <p>{title}</p>
        </div>
    );
};

export default CategoryChip;