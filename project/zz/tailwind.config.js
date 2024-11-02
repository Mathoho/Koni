/** @type {import('tailwindcss').Config} */
module.exports = {
  content: [
    "./src/**/*.{js,jsx,ts,tsx}",
  ],
  theme: {
    extend: {
      colors: {
        "d-purple": "#890B82",
        "d-blue": "#0F0660",
        "d-purple-400": "#2D21AA",
        "l-pink": "#DB499C"
      },
      fontFamily: {
        lilita: ["Lilita One"],
        inter: ["Inter"]
      }
    }
  },
  plugins: [],
}
