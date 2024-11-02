import React from "react";
import TextField from "../components/TextField";
import TextButton from "../components/TextButton";

const SignInPage = () => {
    return(
    <div className="h-screen w-screen flex justify-center items-center">
        <div className="h-3/4 w-3/4 rounded-2xl overflow-hidden flex shadow-2xl bg-white">
			<div className="h-full w-2/5">
				<img className="w-auto h-full object-cover" src="/signin_bg.png" alt="Background" />
			</div>

			<div className="w-3/5 py-[80px]">
				<div className="flex flex-col h-full">
					<h1 className="text-5xl font-lilita ml-[90px]">Sign In</h1>

					<div className="flex flex-col items-center min-h-[30%] w-full mt-[100px]">
						<TextField type="text" placeholder="Username"/>
						<TextField type="password" placeholder="Password"/>
						<TextButton text="Sign In"/>
					</div>
					<div className="flex justify-evenly">
						<a href="#">Don't have an account?</a>
						<a href="#">Forgot Password?</a>
					</div>
				</div>
			</div>

        </div>
    </div>
    );
};

export default SignInPage;