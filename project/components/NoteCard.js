import React from "react";
import OptionsButton from "./OptionsButton";
import CategoryChip from "./CategoryChip";

const NoteCard = (props) => {
    const {title, text, categories, lastEdited} = props

    return (
        <div className="pl-[20px] pt-[20px] pr-[10px] w-[380px] h-[250px] rounded-xl flex flex-col shadow-md shadow-gray-400 overflow-hidden">
            <div className="flex justify-between">
                <h2 className="font-inter text-2xl">{title}</h2>

                <OptionsButton />
            </div>

            <div className="max-h-[50%] h-[50%] overflow-y-clip mt-[8px] flex-wrap">
                <p>{text}</p>
            </div>
            <div className="flex mt-[15px] flex-wrap gap-[5px]">
                {categories.map((categoryName) => (
                        <CategoryChip title={categoryName} />
                ))}
            </div>
            <div className="flex justify-end mt-[10px] mb-[10px] pr-[10px]">
                <p className="bg-gradient-to-tr from-l-pink to-d-blue px-[8px] rounded-xl text-white">{lastEdited}</p>
            </div>
        </div>
    );
};

export default NoteCard;