import react from "@vitejs/plugin-react";
import {defineConfig} from "vite";

// https://vite.dev/config/
export default defineConfig({
    plugins: [react()],
    build: {
        target: "esnext",
        outDir: "dist",
        rollupOptions: {
            output: {
                format: "es",
                entryFileNames: "[name].js",
                chunkFileNames: "[name]-[hash].js",
            }
        },
    },
    worker: {
        format: "es",
        rollupOptions: {
            output: {
                entryFileNames: "[name].js",
                chunkFileNames: "[name]-[hash].js",
            }
        }
    }
});
