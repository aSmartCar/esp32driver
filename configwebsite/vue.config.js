// const BundleAnalyzerPlugin = require('webpack-bundle-analyzer').BundleAnalyzerPlugin;
const CompressionPlugin = require('compression-webpack-plugin');

module.exports = {
    filenameHashing: false,
    productionSourceMap: false,
    // configureWebpack: {
    //     plugins: [new BundleAnalyzerPlugin()]
    // }
    chainWebpack(config) {
        config.plugins.delete('prefetch');
        
        // and this line 
        config.plugin('CompressionPlugin').use(CompressionPlugin);
      }
}
