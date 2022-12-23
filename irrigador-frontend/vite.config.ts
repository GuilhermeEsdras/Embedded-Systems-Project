/// <reference types="vite/client" />

import react from "@vitejs/plugin-react-swc";
import * as path from "path";
import { defineConfig, splitVendorChunkPlugin } from "vite";
import eslint from "vite-plugin-eslint";

const config = () => {
  return defineConfig({
    plugins: [react(), eslint(), splitVendorChunkPlugin()],
    base: "./",
    resolve: {
      alias: {
        "~": path.resolve(__dirname, "./src"),
      },
    },
    server: {
      host: "localhost",
      port: 5173,
      base: "/",
    },
    esbuild: {
      loader: "tsx",
      logOverride: { "this-is-undefined-in-esm": "silent" },
    },
    build: {
      outDir: "build",
      watch: {
        clearScreen: true,
        buildDelay: 0,
        include: "src/**",
        exclude: "node_modules/**",
      },
    },
  });
};

export default config;
