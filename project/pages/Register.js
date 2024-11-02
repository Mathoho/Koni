
import React from "react";
import TextField from "../components/TextField";
import TextButton from "../components/TextButton";

const RegisterPage = () => {
    return(
    <div className="h-screen w-screen flex justify-center items-center">
        <div className="h-3/4 w-3/4 rounded-2xl overflow-hidden flex shadow-2xl bg-white">
			<div className="h-full w-2/5">
				<img className="w-auto h-full object-cover" src="/signin_bg.png" alt="Background" />
			</div>

			<div className="w-3/5 py-[80px]">
				<div className="flex flex-col h-full">
					<h1 className="text-5xl font-lilita ml-[90px]">Register</h1>

					<div className="flex flex-col items-center min-h-[40%] w-full mt-[100px]">
						<TextField type="text" placeholder="Username"/>
						<TextField type="password" placeholder="Password"/>
						<TextField type="password" placeholder="Confirm Password"/>
                        <TextButton text="Register"/>
					</div>
				</div>
			</div>

        </div>
    </div>
    );
};

export default RegisterPage;